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

static INT_PTR CALLBACK CreateWinProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_INITDIALOG: {
            // Store the custom structure passed via DialogBoxIndirectParamW
            SetWindowLongPtr(hDlg, DWLP_USER, lParam);

            // Menu constant values from main.cpp:
            // MENU_DOT = 2, MENU_LINE = 3, MENU_RECT = 4, MENU_ELIPSE = 5
            if (chosen_shape == 2) { 
                CreateWindowExW(0, L"STATIC", L"Позиція X: ",
                    WS_CHILD | WS_VISIBLE | SS_LEFT,
                    20, 20, 100, 25, hDlg, (HMENU)MSG_1, GetModuleHandle(NULL), NULL);

                CreateWindowExW(0, L"STATIC", L"Позиція Y: ",
                    WS_CHILD | WS_VISIBLE | SS_LEFT,
                    20, 45, 100, 25, hDlg, (HMENU)MSG_2, GetModuleHandle(NULL), NULL);
            }
            if (chosen_shape == 3){
                CreateWindowExW(0, L"STATIC", L"Початок X: ",
                    WS_CHILD | WS_VISIBLE | SS_LEFT,
                    20, 20, 100, 25, hDlg, (HMENU)MSG_1, GetModuleHandle(NULL), NULL);

                CreateWindowExW(0, L"STATIC", L"Початок Y: ",
                    WS_CHILD | WS_VISIBLE | SS_LEFT,
                    20, 45, 100, 25, hDlg, (HMENU)MSG_2, GetModuleHandle(NULL), NULL);
                CreateWindowExW(0, L"STATIC", L"Кінець X: ",
                    WS_CHILD | WS_VISIBLE | SS_LEFT,
                    20, 70, 100, 25, hDlg, (HMENU)MSG_3, GetModuleHandle(NULL), NULL);

                CreateWindowExW(0, L"STATIC", L"Кінець Y: ",
                    WS_CHILD | WS_VISIBLE | SS_LEFT,
                    20, 95, 100, 25, hDlg, (HMENU)MSG_4, GetModuleHandle(NULL), NULL);
            }
            if (chosen_shape == 4){
                CreateWindowExW(0, L"STATIC", L"Центр X: ",
                    WS_CHILD | WS_VISIBLE | SS_LEFT,
                    20, 20, 100, 25, hDlg, (HMENU)MSG_1, GetModuleHandle(NULL), NULL);

                CreateWindowExW(0, L"STATIC", L"Центр Y: ",
                    WS_CHILD | WS_VISIBLE | SS_LEFT,
                    20, 45, 100, 25, hDlg, (HMENU)MSG_2, GetModuleHandle(NULL), NULL);
                CreateWindowExW(0, L"STATIC", L"Край X: ",
                    WS_CHILD | WS_VISIBLE | SS_LEFT,
                    20, 70, 100, 25, hDlg, (HMENU)MSG_3, GetModuleHandle(NULL), NULL);

                CreateWindowExW(0, L"STATIC", L"Край Y: ",
                    WS_CHILD | WS_VISIBLE | SS_LEFT,
                    20, 95, 100, 25, hDlg, (HMENU)MSG_4, GetModuleHandle(NULL), NULL);
            }
            if (chosen_shape == 5){
                CreateWindowExW(0, L"STATIC", L"Ліво:",
                    WS_CHILD | WS_VISIBLE | SS_LEFT,
                    20, 20, 100, 25, hDlg, (HMENU)MSG_1, GetModuleHandle(NULL), NULL);

                CreateWindowExW(0, L"STATIC", L"Гора: ",
                    WS_CHILD | WS_VISIBLE | SS_LEFT,
                    20, 45, 100, 25, hDlg, (HMENU)MSG_2, GetModuleHandle(NULL), NULL);
                CreateWindowExW(0, L"STATIC", L"Право: ",
                    WS_CHILD | WS_VISIBLE | SS_LEFT,
                    20, 70, 100, 25, hDlg, (HMENU)MSG_3, GetModuleHandle(NULL), NULL);

                CreateWindowExW(0, L"STATIC", L"Низ: ",
                    WS_CHILD | WS_VISIBLE | SS_LEFT,
                    20, 95, 100, 25, hDlg, (HMENU)MSG_4, GetModuleHandle(NULL), NULL);
            }
            CreateWindowExW(
                WS_EX_CLIENTEDGE,       // Sunken border appearance
                L"EDIT",               // Pre-defined class for text boxes
                L"0",                  // Default initial text
                WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER, // ES_NUMBER restricts input to digits
                130, 20, 130, 25,      // Positioned to the right of the label
                hDlg, 
                (HMENU)DATA_1,     // Control ID (e.g., #define TXT_NUMBER 1002)
                GetModuleHandle(NULL), 
                NULL
            );
            CreateWindowExW(
                WS_EX_CLIENTEDGE,       // Sunken border appearance
                L"EDIT",               // Pre-defined class for text boxes
                L"0",                  // Default initial text
                WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER, // ES_NUMBER restricts input to digits
                130, 45, 130, 25,      // Positioned to the right of the label
                hDlg, 
                (HMENU)DATA_2,     // Control ID (e.g., #define TXT_NUMBER 1002)
                GetModuleHandle(NULL), 
                NULL
            );
            CreateWindowExW(
                WS_EX_CLIENTEDGE,       // Sunken border appearance
                L"EDIT",               // Pre-defined class for text boxes
                L"0",                  // Default initial text
                WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER, // ES_NUMBER restricts input to digits
                130, 70, 130, 25,      // Positioned to the right of the label
                hDlg, 
                (HMENU)DATA_3,     // Control ID (e.g., #define TXT_NUMBER 1002)
                GetModuleHandle(NULL), 
                NULL
            );
            CreateWindowExW(
                WS_EX_CLIENTEDGE,       // Sunken border appearance
                L"EDIT",               // Pre-defined class for text boxes
                L"0",                  // Default initial text
                WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER, // ES_NUMBER restricts input to digits
                130, 95, 130, 25,      // Positioned to the right of the label
                hDlg, 
                (HMENU)DATA_4,     // Control ID (e.g., #define TXT_NUMBER 1002)
                GetModuleHandle(NULL), 
                NULL
            );



            // Accept Button
            CreateWindowExW(0, L"BUTTON", L"Створити",
                WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                30, 140, 100, 30, hDlg, (HMENU)BTN_OK, GetModuleHandle(NULL), NULL);

            // Cancel Button
            CreateWindowExW(0, L"BUTTON", L"Відміна",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                150, 140, 100, 30, hDlg, (HMENU)BTN_CANCEL, GetModuleHandle(NULL), NULL);

            return TRUE;
        }
        case WM_COMMAND: {
            int id = LOWORD(wParam);
            ArrData* data = (ArrData*)GetWindowLongPtr(hDlg, DWLP_USER);

            if (id == BTN_OK) {
                BOOL lpTranslated;
                int data1 = GetDlgItemInt(hDlg, DATA_1, &lpTranslated, FALSE);
                int data2 = GetDlgItemInt(hDlg, DATA_2, &lpTranslated, FALSE);
                int data3 = GetDlgItemInt(hDlg, DATA_3, &lpTranslated, FALSE);
                int data4 = GetDlgItemInt(hDlg, DATA_4, &lpTranslated, FALSE);
                Vector2 vec1 = Vector2{data1,data2};
                Vector2 vec2 = Vector2{data3,data4};
                Shape* shape = nullptr;
                if(lpTranslated){
                    switch(chosen_shape){
                        case 2:
                            shape = new Dot(vec1);
                            break;
                        case 3:
                            shape = new Line(vec1,vec2);
                            break;
                        case 4:
                            shape = new Rect(vec1,vec2);
                            break;
                        case 5:
                            shape = new Elipse(vec1,vec2);
                            break;
                    }
                }
                data->array[*(data->size)] = shape;
                *(data->size) += 1;
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

int create_shape(HWND hwnd, int shape, Shape** arr, int* size) {
    // Allocate buffer for DLGTEMPLATE structure + 3 WORDs (menu, class, title)
    BYTE buffer[sizeof(DLGTEMPLATE) + 3 * sizeof(WORD)] = { 0 };
    LPDLGTEMPLATE pdt = (LPDLGTEMPLATE)buffer;

    pdt->style = WS_POPUP | WS_CAPTION | WS_SYSMENU | DS_MODALFRAME | DS_CENTER;
    pdt->cdit = 0; // Dynamic creation of controls via WM_INITDIALOG
    pdt->x = 0;
    pdt->y = 0;
    pdt->cx = 200; 
    pdt->cy = 100;

    chosen_shape = shape;

    ArrData arr_data;
    arr_data.array = arr;
    arr_data.size = size;

    return (int)DialogBoxIndirectParamW(
        GetModuleHandle(NULL), 
        pdt, 
        hwnd, 
        CreateWinProc, 
        (LPARAM)&arr_data
    );
}