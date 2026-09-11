#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include "module1.h"

#define TXT_INPUT  301
#define BTN_OK     302
#define BTN_CANCEL 303

static INT_PTR CALLBACK Module1Proc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
    wchar_t* buf = (wchar_t*)GetWindowLongPtr(hDlg, DWLP_USER);

    switch (msg) {
        case WM_INITDIALOG: {
            buf = (wchar_t*)lParam;
            SetWindowLongPtr(hDlg, DWLP_USER, (LONG_PTR)buf);
            // Create Input Box
            CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", buf,
                WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
                20, 20, 240, 25, hDlg, (HMENU)TXT_INPUT, GetModuleHandle(NULL), NULL);
            // Accept Button
            CreateWindowExW(0, L"BUTTON", L"Accept",
                WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                30, 60, 100, 30, hDlg, (HMENU)BTN_OK, GetModuleHandle(NULL), NULL);
            // Cancel Button
            CreateWindowExW(0, L"BUTTON", L"Cancel",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                150, 60, 100, 30, hDlg, (HMENU)BTN_CANCEL, GetModuleHandle(NULL), NULL);
            return TRUE;
        }

        case WM_COMMAND: {
            int id = LOWORD(wParam);
            if (id == BTN_OK) {
                if (buf) {
                    GetDlgItemTextW(hDlg, TXT_INPUT, buf, 128);
                }
                EndDialog(hDlg, IDOK);
                return TRUE;
            } 
            else if (id == BTN_CANCEL || id == IDCANCEL) {
                EndDialog(hDlg, IDCANCEL);
                return TRUE;
            }
            break;
        }
    }
    return FALSE;
}

int module1_func(HWND hwnd,  wchar_t* buffer_ptr){
    BYTE buffer[512] = { 0 };
    LPDLGTEMPLATE pdt = (LPDLGTEMPLATE)buffer;
    pdt->style = WS_POPUP | WS_CAPTION | WS_SYSMENU | DS_MODALFRAME | DS_CENTER;
    pdt->cx = 150; 
    pdt->cy = 60;

    return (DialogBoxIndirectParamW(GetModuleHandle(NULL), pdt, hwnd, Module1Proc, (LPARAM)buffer_ptr) == IDOK);
}