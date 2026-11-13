#include "dialogue.h"
#include "../send_data.h"   // GenData, StartWork
#include <cerrno>
#include <climits>
#include <cwchar>
#include <execution>
#include "../helpers.h"
#include "lab6_win.h"

void Dialogue::OnCreateControls() {
    const DWORD editStyle = WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL;

    AddControl(L"STATIC", L"n:",   SS_LEFT, 15, 20, 50, 20, -1);
    AddControl(L"EDIT",   L"10",   editStyle, 75, 17, 150, 22, IDC_EDIT_N);

    AddControl(L"STATIC", L"min:", SS_LEFT, 15, 52, 50, 20, -1);
    AddControl(L"EDIT",   L"0",    editStyle, 75, 49, 150, 22, IDC_EDIT_MIN);

    AddControl(L"STATIC", L"max:", SS_LEFT, 15, 84, 50, 20, -1);
    AddControl(L"EDIT",   L"100",  editStyle, 75, 81, 150, 22, IDC_EDIT_MAX);

    AddControl(L"BUTTON", L"OK",       WS_TABSTOP | BS_DEFPUSHBUTTON, 40,  120, 80, 26, IDOK);
    AddControl(L"BUTTON", L"Скасувати", WS_TABSTOP | BS_PUSHBUTTON,   130, 120, 90, 26, IDCANCEL);
}

bool Dialogue::ReadInt(int controlId, int& value) const {
    wchar_t buf[32] = {};
    GetDlgItemTextW(m_hwnd, controlId, buf, 32);

    wchar_t* end = nullptr;
    errno = 0;
    long v = wcstol(buf, &end, 10);

    // Порожній рядок, сміття після числа або переповнення — помилка
    if (end == buf || *end != L'\0' || errno == ERANGE || v < INT_MIN || v > INT_MAX) {
        return false;
    }
    value = static_cast<int>(v);
    return true;
}
extern ChildProcessInfo proc2, proc3;

static HWND WaitForMainWindow(const ChildProcessInfo& proc, int timeoutMs = 3000) {
    // Чекаємо, поки процес створить чергу повідомлень
    WaitForInputIdle(proc.pi.hProcess, timeoutMs);

    for (int waited = 0; waited < timeoutMs; waited += 50) {
        HWND h = FindMainWindowForProcess(proc.pi.dwProcessId);
        if (h) return h;
        Sleep(50);
    }
    return nullptr;
}

void StartWork(GenData data, HWND m_hwnd) {
    wchar_t args[32];
    swprintf_s(args, L"%llu", (unsigned long long)(UINT_PTR)m_hwnd);
    if (!LaunchChildProcess(L"obj2.exe", args, proc2)) return;
    if (!LaunchChildProcess(L"obj3.exe", args, proc3)) return;

    if (proc2.isRunning && proc3.isRunning) {
        ArrangeObjectWindows(proc2.pi.dwProcessId, proc3.pi.dwProcessId);
    }

    HWND obj2 = WaitForMainWindow(proc2);
    if (!obj2) {
        MessageBoxW(nullptr, L"Не знайдено вікно obj2", L"Помилка", MB_OK | MB_ICONERROR);
        return;
    }
    //MessageBoxW(m_hwnd, L"Відправляємо дані до об'єкт2.", L"Помилка", MB_OK | MB_ICONWARNING);
    SendDataToObject(obj2, &data);
}

void Dialogue::OnOk() {
    int n = 0, mn = 0, mx = 0;

    if (!ReadInt(IDC_EDIT_N, n)) {
        MessageBoxW(m_hwnd, L"Поле n має містити ціле число.", L"Помилка", MB_OK | MB_ICONWARNING);
        SetFocus(GetDlgItem(m_hwnd, IDC_EDIT_N));
        return;
    }
    if (!ReadInt(IDC_EDIT_MIN, mn)) {
        MessageBoxW(m_hwnd, L"Поле min має містити ціле число.", L"Помилка", MB_OK | MB_ICONWARNING);
        SetFocus(GetDlgItem(m_hwnd, IDC_EDIT_MIN));
        return;
    }
    if (!ReadInt(IDC_EDIT_MAX, mx)) {
        MessageBoxW(m_hwnd, L"Поле max має містити ціле число.", L"Помилка", MB_OK | MB_ICONWARNING);
        SetFocus(GetDlgItem(m_hwnd, IDC_EDIT_MAX));
        return;
    }
    if (n <= 0) {
        MessageBoxW(m_hwnd, L"n має бути більше нуля.", L"Помилка", MB_OK | MB_ICONWARNING);
        SetFocus(GetDlgItem(m_hwnd, IDC_EDIT_N));
        return;
    }
    if (mn > mx) {
        MessageBoxW(m_hwnd, L"min не може бути більшим за max.", L"Помилка", MB_OK | MB_ICONWARNING);
        SetFocus(GetDlgItem(m_hwnd, IDC_EDIT_MIN));
        return;
    }

    GenData data = GenData{ n, mn, mx };
    parent_ptr->data = data;
    StartWork(data,m_owner);   // StartWork забирає володіння вказівником
    Close();
}

LRESULT Dialogue::OnCommand(int id, int eventCode, HWND controlHandle) {
    if (id == IDOK) {          // кнопка OK або Enter (через IsDialogMessage)
        OnOk();
        return 0;
    }
    // IDCANCEL (кнопка і Esc) обробляє базовий клас — закриває вікно
    return DialogueWindow::OnCommand(id, eventCode, controlHandle);
}
