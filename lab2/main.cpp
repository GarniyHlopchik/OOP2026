#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <windowsx.h> // Required for GET_X_LPARAM and GET_Y_LPARAM
#include "shapes/shape.h"
#include "shapes/elipse.h"
#include "shapes/vec2.h"
#include "create_win.h"

#define MENU_FILE    1
#define MENU_DOT     2
#define MENU_LINE    3
#define MENU_RECT    4
#define MENU_ELIPSE  5
#define MENU_ABOUT   6

Shape** ShapeArray;
int arr_size = 0;
HWND hModelessDlg = NULL;
Shape* PreviewShape;
bool creation_state = false;
bool isDragging = false;
int shape_id;
Vector2 start_pos;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;
    switch (msg) {
        case WM_DIALOG_CLOSED: {
            hModelessDlg = NULL; // Reset handle so a new dialogue can be created
            PreviewShape = nullptr;
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            switch (wmId) {
                case MENU_DOT:
                case MENU_LINE:
                case MENU_ELIPSE:
                case MENU_RECT:
                    if(!hModelessDlg){
                        //hModelessDlg = create_shape(hwnd, wmId,ShapeArray,&arr_size);
                        creation_state = true;
                        shape_id = wmId;
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                    break;
                case MENU_ABOUT:
                    MessageBox(hwnd, L"Друга лабораторна з ООП", L"About", MB_OK | MB_ICONINFORMATION);
                    break;
            }
            break;
        }
        case WM_LBUTTONDOWN:{
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            start_pos = Vector2{xPos,yPos};
            PreviewShape = CreateShapeObject(Vector2{xPos,yPos},Vector2{xPos,yPos},shape_id);
            isDragging = true;
            SetCapture(hwnd);
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }
        case WM_MOUSEMOVE: {
            // 2. TRACK: Only record movement if currently in a drag state
            if (isDragging) {
                Vector2 pos;
                pos.x = GET_X_LPARAM(lParam);
                pos.y = GET_Y_LPARAM(lParam);

                PreviewShape->set_second(pos);

                // Example: Trigger a window repaint to show a selection box or line
                InvalidateRect(hwnd, NULL, TRUE);
            }
            return 0;
        }
        case WM_LBUTTONUP: {
            // 3. FINISH: End tracking when button is released
            if (isDragging) {
                isDragging = false;
                creation_state = false;
                
                // Always release capture when finished
                ReleaseCapture();

                // Final position when released
                int endX = GET_X_LPARAM(lParam);
                int endY = GET_Y_LPARAM(lParam);

                delete PreviewShape;
                PreviewShape = nullptr;

                ShapeArray[arr_size] = CreateShapeObject(start_pos,Vector2{endX,endY},shape_id);
                arr_size+=1;
                // Perform final action (e.g., commit selection, drop object)

                InvalidateRect(hwnd, NULL, TRUE);
            }
            return 0;
        }
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            //draw here
            for(int i = 0; i<arr_size; i++){
                ShapeArray[i]->draw(hdc);
            }
            if(PreviewShape != nullptr){
                PreviewShape->preview_draw(hdc);
            }
            
            EndPaint(hwnd, &ps);
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam); // Default handling
    }
    return 0;
}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    ShapeArray = new Shape*[103];
    //win class
    const wchar_t className[] = L"LAB2";
    WNDCLASS winClass{
        .lpfnWndProc = WndProc,
        .hInstance = hInstance,
        .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
        .lpszClassName = className
    };
    RegisterClass(&winClass);
    //building menu
    HMENU hMenu = CreateMenu();
    AppendMenuW(hMenu, MF_STRING, MENU_FILE, L"Файл");
    HMENU hActionMenu = CreatePopupMenu();
    AppendMenuW(hActionMenu, MF_STRING, MENU_DOT, L"Крапка");
    AppendMenuW(hActionMenu, MF_STRING, MENU_LINE, L"Лінія");
    AppendMenuW(hActionMenu, MF_STRING, MENU_RECT, L"Прямокутник");
    AppendMenuW(hActionMenu, MF_STRING, MENU_ELIPSE, L"Еліпс");
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hActionMenu, L"Об'єкти");
    AppendMenuW(hMenu, MF_STRING, MENU_ABOUT, L"Довідка");
    //building window
    HWND hwnd = CreateWindowEx(
        0,
        className,
        L"Lab2 main",
        WS_OVERLAPPEDWINDOW,
        100, 100, //pos
        640, 480, //size
        NULL, //parent
        hMenu, //menu
        hInstance,
        NULL
    );
    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);

    //msg loop
    BOOL msgReturn;
    MSG msg = { };
    while ((msgReturn = GetMessage(&msg, NULL, 0, 0)) != 0) {
        if(msgReturn == -1){
            break;
        }
        if (hModelessDlg == NULL || !IsDialogMessage(hModelessDlg, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    for (int i = 0; i < arr_size; i++) {
        delete ShapeArray[i];
    }
    delete[] ShapeArray; 
    return 0;
}