#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <windows.h>
#include <commctrl.h>

class Window {
public:
    Window() : m_hwnd(nullptr) {}
    virtual ~Window() {
        if (m_hwnd && IsWindow(m_hwnd)) {
            DestroyWindow(m_hwnd);
        }
    }

    // Метод створення вікна
    virtual bool Create(PCWSTR title, DWORD style, DWORD exStyle = 0,
                int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
                int width = CW_USEDEFAULT, int height = CW_USEDEFAULT,
                HWND parent = nullptr, HMENU menu = nullptr) 
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

    void Show(int cmdShow = SW_SHOW) {
        ShowWindow(m_hwnd, cmdShow);
        UpdateWindow(m_hwnd);
    }

    HWND GetHandle() const { return m_hwnd; }

protected:
    // Головний диспетчер повідомлень класу
    virtual LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
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

    // Обробник повідомлення WM_NOTIFY (Метод-член класу)
    virtual LRESULT OnNotify(int controlId, NMHDR* pNotificationHeader) {
        // Базова реалізація (можна перевизначити у похідних класах)
        return 0;
    }

    // Обробник команд (кнопки, меню тощо)
    virtual LRESULT OnCommand(int id, int eventCode, HWND controlHandle) {
        return 0;
    }

    // Обробник малювання
    virtual void OnPaint() {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(m_hwnd, &ps);
        EndPaint(m_hwnd, &ps);
    }

    // Обробник закриття/знищення вікна
    virtual void OnDestroy() {
        PostQuitMessage(0);
    }

    virtual PCWSTR GetClassName() const { return L"BaseWindowClass"; }
    HWND m_hwnd;
private:
    

    // Внутрішній статичний метод-місток для зв'язку WinAPI та C++ об'єкта
    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
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
};

#endif