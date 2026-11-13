#include "obj2_win.h"
#include <minwindef.h>
#include <winuser.h>
#include "../send_data.h"
#include <vector>
#include <random>
#include <cwchar>
#include <iostream>

LRESULT OBJ2Window::OnCommand(int id, int eventCode, HWND controlHandle){
    if (eventCode == BN_CLICKED) {
        switch(id){

        }
    }
    return Window::OnCommand(id, eventCode, controlHandle);
}
void GenNumbers(int n, int min, int max, std::vector<double>& vec) {
    static std::mt19937 gen{ std::random_device{}() };
    std::uniform_real_distribution<double> dist(min, max);

    vec.reserve(vec.size() + n);
    for (int i = 0; i < n; i++) {
        vec.push_back(dist(gen));
    }
}
bool CopyTextToClipboard(HWND owner, const std::wstring& text) {
    if (!OpenClipboard(owner)) return false;
    EmptyClipboard();

    size_t bytes = (text.size() + 1) * sizeof(wchar_t);
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, bytes);
    if (!hMem) { CloseClipboard(); return false; }

    void* p = GlobalLock(hMem);
    memcpy(p, text.c_str(), bytes);
    GlobalUnlock(hMem);

    // Після успішного SetClipboardData пам'яттю володіє система, GlobalFree не викликати
    if (!SetClipboardData(CF_UNICODETEXT, hMem)) {
        GlobalFree(hMem);
        CloseClipboard();
        return false;
    }
    CloseClipboard();
    return true;
}
LRESULT OBJ2Window::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam){
    switch(msg){
        case WM_PAINT:
                    OnPaint();
                    return 0;
        case WM_COPYDATA:{
            numbers.clear();
            COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)lParam;

                    int commandType = (int)pcds->dwData;     // Яку команду передали
                    int byteSize = pcds->cbData;             // Скільки байт прийшло
                    GenData* incomingData = (GenData*)pcds->lpData;  // Сам масив даних

                    int elementCount = 1;

                        GenNumbers(incomingData->n, incomingData->min, incomingData->max, numbers);

                        InvalidateRect(m_hwnd, NULL, TRUE);
                        std::wstring text;
                        wchar_t buf[64];
                        for (double v : numbers) {
                            swprintf_s(buf, L"%.3f\r\n", v);
                            text += buf;
                        }
                        CopyTextToClipboard(m_hwnd, text);
                        SendMessageW(parent, WM_APP + 1, (WPARAM/*дані*/)(101), (LPARAM/*додатково*/)(0));


                    return TRUE; // Повертаємо TRUE, що означає успішне опрацювання
        }
    }
    return Window::HandleMessage(msg, wParam, lParam);
}
#define NUM_COLS 4
void OBJ2Window::OnPaint() {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hwnd, &ps);

    SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
    SetBkMode(hdc, TRANSPARENT);

    // Висота рядка береться зі шрифту
    TEXTMETRICW tm;
    GetTextMetricsW(hdc, &tm);
    const int rowHeight = tm.tmHeight + 6;

    // Ширина колонки: клієнтська область ділиться порівну
    RECT client;
    GetClientRect(m_hwnd, &client);
    const int margin = 10;
    const int colWidth = (client.right - 2 * margin) / NUM_COLS;

    for (size_t i = 0; i < numbers.size(); i++) {
        int row = static_cast<int>(i / NUM_COLS);
        int col = static_cast<int>(i % NUM_COLS);

        RECT cell;
        cell.left   = margin + col * colWidth;
        cell.top    = margin + row * rowHeight;
        cell.right  = cell.left + colWidth;
        cell.bottom = cell.top + rowHeight;

        // Пропускаємо клітинки, які не потрапляють у область перемальовування
        RECT tmp;
        if (!IntersectRect(&tmp, &cell, &ps.rcPaint)) continue;

        wchar_t buf[64];
        swprintf_s(buf, L"%.3f", numbers[i]);
        DrawTextW(hdc, buf, -1, &cell, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
    }

     EndPaint(m_hwnd, &ps);
}
