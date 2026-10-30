#include "window.h"

Window::Window() : m_hwnd(nullptr) {}

Window::~Window() {
    if (m_hwnd && IsWindow(m_hwnd)) {
        DestroyWindow(m_hwnd);
    }
}

bool Window::Create(PCWSTR title, DWORD style, DWORD exStyle,
                    int x, int y, int width, int height,
                    HWND parent, HMENU menu)
{
    WNDCLASS wc = {};
    wc.lpfnWndProc   = Window::StaticWndProc; // Використовуємо внутрішній приватний місток
    wc.hInstance     = GetModuleHandle(nullptr);
    wc.lpszClassName = GetClassName();
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);

    RegisterClass(&wc);

    m_hwnd = CreateWindowEx(
        exStyle, GetClassName(), title, style,
        x, y, width, height,
        parent, menu, GetModuleHandle(nullptr),
        this // Передаємо 'this' як параметр створення (lpParam)
    );

    return m_hwnd != nullptr;
}

void Window::Show(int cmdShow) {
    ShowWindow(m_hwnd, cmdShow);
    UpdateWindow(m_hwnd);
}

LRESULT Window::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_NOTIFY:
            // Виклик обробника OnNotify як функції-члена
            return OnNotify((int)wParam, reinterpret_cast<NMHDR*>(lParam));

        case WM_COMMAND:
            return OnCommand(LOWORD(wParam), HIWORD(wParam), (HWND)lParam);

        case WM_PAINT:
            OnPaint();
            return 0;

        case WM_DESTROY:
            OnDestroy();
            return 0;
    }
    return DefWindowProc(m_hwnd, msg, wParam, lParam);
}

LRESULT Window::OnNotify(int controlId, NMHDR* pNotificationHeader) {
    // Базова реалізація (можна перевизначити у похідних класах)
    return 0;
}

LRESULT Window::OnCommand(int id, int eventCode, HWND controlHandle) {
    return 0;
}

void Window::OnPaint() {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hwnd, &ps);
    EndPaint(m_hwnd, &ps);
}

void Window::OnDestroy() {
    PostQuitMessage(0);
}

PCWSTR Window::GetClassName() const {
    return L"BaseWindowClass";
}

LRESULT CALLBACK Window::StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    Window* pThis = nullptr;

    if (msg == WM_NCCREATE) {
        // Отримуємо вказівник на екземпляр класу 'this', який ми передали в CreateWindowEx
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<Window*>(pCreate->lpCreateParams);

        // Зберігаємо вказівник у внутрішній пам'яті самого вікна WinAPI
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        pThis->m_hwnd = hwnd;
    } else {
        // Отримуємо збережений вказівник 'this' для всіх наступних повідомлень
        pThis = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis) {
        return pThis->HandleMessage(msg, wParam, lParam);
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}