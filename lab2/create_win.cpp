#include "create_win.h"
#include "shapes/shape.h"
#include "shapes/dot.h"
#include "shapes/line.h"
#include "shapes/rect.h"
#include "shapes/elipse.h"
#include "shapes/vec2.h"

static int chosen_shape;

#define MSG_1 1000
#define MSG_2 1001
#define MSG_3 1002
#define MSG_4 1003

#define DATA_1 1004
#define DATA_2 1005
#define DATA_3 1006
#define DATA_4 1007

#define BTN_OK     1008
#define BTN_CANCEL 1009

struct DialogContext {
    HWND hMainWnd;
    ArrData arrData;
};
extern Shape* PreviewShape;
Shape* CreateShapeObject(Vector2 vec1, Vector2 vec2){
    Shape* shape;
    switch (chosen_shape) {
        case 2: shape = new Dot(vec1); break;
        case 3: shape = new Line(vec1, vec2); break;
        case 4: shape = new Rect(vec1, vec2); break;
        case 5: shape = new Elipse(vec1, vec2); break;
    }
    return shape;
}
static INT_PTR CALLBACK CreateWinProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_INITDIALOG: {
            DialogContext* ctx = (DialogContext*)lParam;
            SetWindowLongPtr(hDlg, DWLP_USER, (LONG_PTR)ctx);

            if (chosen_shape == 2) { 
                CreateWindowExW(0, L"STATIC", L"Позиція X: ", WS_CHILD | WS_VISIBLE | SS_LEFT, 20, 20, 100, 25, hDlg, (HMENU)MSG_1, GetModuleHandle(NULL), NULL);
                CreateWindowExW(0, L"STATIC", L"Позиція Y: ", WS_CHILD | WS_VISIBLE | SS_LEFT, 20, 45, 100, 25, hDlg, (HMENU)MSG_2, GetModuleHandle(NULL), NULL);
            }
            if (chosen_shape == 3){
                CreateWindowExW(0, L"STATIC", L"Початок X: ", WS_CHILD | WS_VISIBLE | SS_LEFT, 20, 20, 100, 25, hDlg, (HMENU)MSG_1, GetModuleHandle(NULL), NULL);
                CreateWindowExW(0, L"STATIC", L"Початок Y: ", WS_CHILD | WS_VISIBLE | SS_LEFT, 20, 45, 100, 25, hDlg, (HMENU)MSG_2, GetModuleHandle(NULL), NULL);
                CreateWindowExW(0, L"STATIC", L"Кінець X: ",  WS_CHILD | WS_VISIBLE | SS_LEFT, 20, 70, 100, 25, hDlg, (HMENU)MSG_3, GetModuleHandle(NULL), NULL);
                CreateWindowExW(0, L"STATIC", L"Кінець Y: ",  WS_CHILD | WS_VISIBLE | SS_LEFT, 20, 95, 100, 25, hDlg, (HMENU)MSG_4, GetModuleHandle(NULL), NULL);
            }
            if (chosen_shape == 4){
                CreateWindowExW(0, L"STATIC", L"Центр X: ",  WS_CHILD | WS_VISIBLE | SS_LEFT, 20, 20, 100, 25, hDlg, (HMENU)MSG_1, GetModuleHandle(NULL), NULL);
                CreateWindowExW(0, L"STATIC", L"Центр Y: ",  WS_CHILD | WS_VISIBLE | SS_LEFT, 20, 45, 100, 25, hDlg, (HMENU)MSG_2, GetModuleHandle(NULL), NULL);
                CreateWindowExW(0, L"STATIC", L"Край X: ",   WS_CHILD | WS_VISIBLE | SS_LEFT, 20, 70, 100, 25, hDlg, (HMENU)MSG_3, GetModuleHandle(NULL), NULL);
                CreateWindowExW(0, L"STATIC", L"Край Y: ",   WS_CHILD | WS_VISIBLE | SS_LEFT, 20, 95, 100, 25, hDlg, (HMENU)MSG_4, GetModuleHandle(NULL), NULL);
            }
            if (chosen_shape == 5){
                CreateWindowExW(0, L"STATIC", L"Ліво:",     WS_CHILD | WS_VISIBLE | SS_LEFT, 20, 20, 100, 25, hDlg, (HMENU)MSG_1, GetModuleHandle(NULL), NULL);
                CreateWindowExW(0, L"STATIC", L"Гора: ",    WS_CHILD | WS_VISIBLE | SS_LEFT, 20, 45, 100, 25, hDlg, (HMENU)MSG_2, GetModuleHandle(NULL), NULL);
                CreateWindowExW(0, L"STATIC", L"Право: ",   WS_CHILD | WS_VISIBLE | SS_LEFT, 20, 70, 100, 25, hDlg, (HMENU)MSG_3, GetModuleHandle(NULL), NULL);
                CreateWindowExW(0, L"STATIC", L"Низ: ",     WS_CHILD | WS_VISIBLE | SS_LEFT, 20, 95, 100, 25, hDlg, (HMENU)MSG_4, GetModuleHandle(NULL), NULL);
            }

            CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"0", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER, 130, 20, 130, 25, hDlg, (HMENU)DATA_1, GetModuleHandle(NULL), NULL);
            CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"0", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER, 130, 45, 130, 25, hDlg, (HMENU)DATA_2, GetModuleHandle(NULL), NULL);
            CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"0", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER, 130, 70, 130, 25, hDlg, (HMENU)DATA_3, GetModuleHandle(NULL), NULL);
            CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"0", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER, 130, 95, 130, 25, hDlg, (HMENU)DATA_4, GetModuleHandle(NULL), NULL);

            CreateWindowExW(0, L"BUTTON", L"Створити", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 30, 140, 100, 30, hDlg, (HMENU)BTN_OK, GetModuleHandle(NULL), NULL);
            CreateWindowExW(0, L"BUTTON", L"Відміна",  WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,    150, 140, 100, 30, hDlg, (HMENU)BTN_CANCEL, GetModuleHandle(NULL), NULL);

            return TRUE;
        }
        case WM_COMMAND: {
            int id = LOWORD(wParam);
            int code = HIWORD(wParam);
            DialogContext* ctx = (DialogContext*)GetWindowLongPtr(hDlg, DWLP_USER);

            if (code == EN_CHANGE && (id == DATA_1 || id == DATA_2 || id == DATA_3 || id == DATA_4)) {
                BOOL lpTranslated;
                int data1 = GetDlgItemInt(hDlg, DATA_1, &lpTranslated, FALSE);
                int data2 = GetDlgItemInt(hDlg, DATA_2, &lpTranslated, FALSE);
                int data3 = GetDlgItemInt(hDlg, DATA_3, &lpTranslated, FALSE);
                int data4 = GetDlgItemInt(hDlg, DATA_4, &lpTranslated, FALSE);
                Vector2 vec1 = Vector2{data1, data2};
                Vector2 vec2 = Vector2{data3, data4};

                // Update a shared preview shape variable accessible by your main window
                PreviewShape = CreateShapeObject(vec1,vec2);

                // Force main window to redraw immediately
                InvalidateRect(ctx->hMainWnd, NULL, TRUE);
                UpdateWindow(ctx->hMainWnd);
                return TRUE;
            }

            if (id == BTN_OK) {
                BOOL lpTranslated;
                int data1 = GetDlgItemInt(hDlg, DATA_1, &lpTranslated, FALSE);
                int data2 = GetDlgItemInt(hDlg, DATA_2, &lpTranslated, FALSE);
                int data3 = GetDlgItemInt(hDlg, DATA_3, &lpTranslated, FALSE);
                int data4 = GetDlgItemInt(hDlg, DATA_4, &lpTranslated, FALSE);
                Vector2 vec1 = Vector2{data1, data2};
                Vector2 vec2 = Vector2{data3, data4};
                Shape* shape = CreateShapeObject(vec1,vec2);

                ctx->arrData.array[*(ctx->arrData.size)] = shape;
                *(ctx->arrData.size) += 1;

                // Force the main window to repaint immediately with the new shape
                InvalidateRect(ctx->hMainWnd, NULL, TRUE);
                UpdateWindow(ctx->hMainWnd);

                DestroyWindow(hDlg);
                return TRUE;
            } 
            else if (id == BTN_CANCEL || id == IDCANCEL) {
                DestroyWindow(hDlg);
                return TRUE;
            }
            break;
        }
        case WM_NCDESTROY: {
            DialogContext* ctx = (DialogContext*)GetWindowLongPtr(hDlg, DWLP_USER);
            if (ctx) {
                // Send a message to the main window notifying it that this dialog is gone
                PostMessage(ctx->hMainWnd, WM_DIALOG_CLOSED, 0, 0);
                delete ctx;
            }
            return TRUE;
        }
    }
    return FALSE;
}

HWND create_shape(HWND hwnd, int shape, Shape** arr, int* size) {
    BYTE buffer[sizeof(DLGTEMPLATE) + 3 * sizeof(WORD)] = { 0 };
    LPDLGTEMPLATE pdt = (LPDLGTEMPLATE)buffer;

    // Notice DS_MODALFRAME is removed so it behaves as modeless
    pdt->style = WS_POPUP | WS_CAPTION | WS_SYSMENU | DS_CENTER | WS_VISIBLE;
    pdt->cdit = 0;
    pdt->x = 0;
    pdt->y = 0;
    pdt->cx = 200; 
    pdt->cy = 100;

    chosen_shape = shape;

    DialogContext* ctx = new DialogContext();
    ctx->hMainWnd = hwnd;
    ctx->arrData.array = arr;
    ctx->arrData.size = size;

    // Switched to CreateDialogIndirectParamW
    return CreateDialogIndirectParamW(
        GetModuleHandle(NULL), 
        pdt, 
        hwnd, 
        CreateWinProc, 
        (LPARAM)ctx
    );
}