#include "dialogue_window.h"
#include <algorithm>

std::vector<DialogueWindow*> DialogueWindow::s_dialogues;

DialogueWindow::DialogueWindow() : m_owner(nullptr) {}

DialogueWindow::~DialogueWindow() {
    // Знищуємо вікно поки об'єкт ще є DialogueWindow, щоб WM_DESTROY
    // потрапив у наш OnDestroy (а не в базовий, який викликає PostQuitMessage).
    if (m_hwnd && IsWindow(m_hwnd)) {
        DestroyWindow(m_hwnd);
    }
    s_dialogues.erase(std::remove(s_dialogues.begin(), s_dialogues.end(), this),
                      s_dialogues.end());
}

bool DialogueWindow::Create(PCWSTR title, DWORD style, DWORD exStyle,
                            int x, int y, int width, int height,
                            HWND parent, HMENU menu)
{
    m_owner = parent;
    style   |= WS_POPUP | WS_CAPTION | WS_SYSMENU;
    exStyle |= WS_EX_CONTROLPARENT;

    if (!Window::Create(title, style, exStyle, x, y, width, height, parent, menu)) {
        return false;
    }

    if (std::find(s_dialogues.begin(), s_dialogues.end(), this) == s_dialogues.end()) {
        s_dialogues.push_back(this);
    }
    return true;
}

bool DialogueWindow::Open(PCWSTR title, int clientWidth, int clientHeight, HWND owner) {
    const DWORD style   = WS_POPUP | WS_CAPTION | WS_SYSMENU;
    const DWORD exStyle = WS_EX_DLGMODALFRAME;

    // Перераховуємо розмір клієнтської області у розмір усього вікна
    RECT rc = { 0, 0, clientWidth, clientHeight };
    AdjustWindowRectEx(&rc, style, FALSE, exStyle);
    int w = rc.right - rc.left;
    int h = rc.bottom - rc.top;

    int x = CW_USEDEFAULT;
    int y = CW_USEDEFAULT;
    if (owner) {
        RECT o;
        GetWindowRect(owner, &o);
        x = o.left + ((o.right - o.left) - w) / 2;
        y = o.top  + ((o.bottom - o.top) - h) / 2;
    }

    if (!Create(title, style, exStyle, x, y, w, h, owner, nullptr)) {
        return false;
    }
    Show();
    return true;
}

void DialogueWindow::Close() {
    if (m_hwnd && IsWindow(m_hwnd)) {
        DestroyWindow(m_hwnd);
    }
}

bool DialogueWindow::IsOpen() const {
    return m_hwnd && IsWindow(m_hwnd);
}

bool DialogueWindow::IsDialogueMessage(MSG* msg) {
    // Копія списку: діалог може закритись (і видалити себе зі списку) під час обробки
    std::vector<DialogueWindow*> copy = s_dialogues;
    for (DialogueWindow* d : copy) {
        if (d->IsOpen() && IsDialogMessageW(d->m_hwnd, msg)) {
            return true;
        }
    }
    return false;
}

void DialogueWindow::OnCreateControls() {
    // За замовчуванням порожньо — перевизначається у похідних класах
}

void DialogueWindow::OnClosed() {
    // За замовчуванням порожньо
}

HWND DialogueWindow::AddControl(PCWSTR className, PCWSTR text, DWORD style,
                                int x, int y, int width, int height, int id)
{
    HWND h = CreateWindowExW(
        0, className, text, WS_CHILD | WS_VISIBLE | style,
        x, y, width, height,
        m_hwnd, (HMENU)(INT_PTR)id, GetModuleHandle(nullptr), nullptr
    );
    if (h) {
        SendMessageW(h, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
    }
    return h;
}

PCWSTR DialogueWindow::GetClassName() const {
    return L"DialogueWindowClass";
}

LRESULT DialogueWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            OnCreateControls();
            return 0;

        case WM_NCDESTROY: {
            // Останнє повідомлення вікна: після нього дескриптор недійсний
            LRESULT result = DefWindowProcW(m_hwnd, msg, wParam, lParam);
            m_hwnd = nullptr;
            return result;
        }
    }
    return Window::HandleMessage(msg, wParam, lParam);
}

LRESULT DialogueWindow::OnCommand(int id, int eventCode, HWND controlHandle) {
    // Esc у IsDialogMessage надсилає WM_COMMAND з IDCANCEL
    if (id == IDCANCEL) {
        Close();
        return 0;
    }
    return Window::OnCommand(id, eventCode, controlHandle);
}

void DialogueWindow::OnDestroy() {
    // Без PostQuitMessage — закриття діалогу не має завершувати програму
    s_dialogues.erase(std::remove(s_dialogues.begin(), s_dialogues.end(), this),
                      s_dialogues.end());
    OnClosed();
}