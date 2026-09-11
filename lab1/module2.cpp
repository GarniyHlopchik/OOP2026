#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include "module2.h"

#define LIST_BOX  401
#define BTN_OK    402
#define BTN_CANCEL 403

static INT_PTR CALLBACK ListDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
    wchar_t* buffer = (wchar_t*)GetWindowLongPtr(hDlg, DWLP_USER);

    switch (msg) {
        case WM_INITDIALOG: {
            buffer = (wchar_t*)lParam;
            SetWindowLongPtr(hDlg, DWLP_USER, (LONG_PTR)buffer);

            // Create ListBox
            HWND hListBox = CreateWindowExW(WS_EX_CLIENTEDGE, L"LISTBOX", NULL,
                WS_CHILD | WS_VISIBLE | LBS_STANDARD,
                20, 20, 240, 100, hDlg, (HMENU)LIST_BOX, GetModuleHandle(NULL), NULL);
            const wchar_t* options[] = { L"IM-53", L"IM-52", L"IM-42", L"IM-67" };
            for (int i = 0; i < 4; ++i) {
                SendMessageW(hListBox, LB_ADDSTRING, 0, (LPARAM)options[i]);
            }
            SendMessageW(hListBox, LB_SETCURSEL, 0, 0); // Default

            CreateWindowExW(0, L"BUTTON", L"Accept",
                WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                30, 135, 100, 30, hDlg, (HMENU)BTN_OK, GetModuleHandle(NULL), NULL);

            CreateWindowExW(0, L"BUTTON", L"Cancel",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                150, 135, 100, 30, hDlg, (HMENU)BTN_CANCEL, GetModuleHandle(NULL), NULL);

            return TRUE;
        }

        case WM_COMMAND: {
            int id = LOWORD(wParam);
            if (id == BTN_OK) {
                if (buffer) {
                    HWND hListBox = GetDlgItem(hDlg, LIST_BOX);
                    int selIndex = (int)SendMessageW(hListBox, LB_GETCURSEL, 0, 0);
                    if (selIndex != LB_ERR) {
                        SendMessageW(hListBox, LB_GETTEXT, selIndex, (LPARAM)buffer);
                    }
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

int module2_func(HWND hwnd,  wchar_t* buffer_ptr){
    BYTE buffer[512] = { 0 };
    LPDLGTEMPLATE pdt = (LPDLGTEMPLATE)buffer;
    pdt->style = WS_POPUP | WS_CAPTION | WS_SYSMENU | DS_MODALFRAME | DS_CENTER;
    pdt->cx = 150; 
    pdt->cy = 100;

    return (DialogBoxIndirectParamW(GetModuleHandle(NULL), pdt, hwnd, ListDlgProc, (LPARAM)buffer_ptr) == IDOK);
}