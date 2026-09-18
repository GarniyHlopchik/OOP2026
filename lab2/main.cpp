#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;
    switch (msg) {
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            switch (wmId) {
                case MENU_DOT:
                case MENU_LINE:
                case MENU_ELIPSE:
                case MENU_RECT:
                    create_shape(hwnd, wmId,ShapeArray,&arr_size);
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case MENU_ABOUT:
                    MessageBox(hwnd, L"Друга лабораторна з ООП", L"About", MB_OK | MB_ICONINFORMATION);
                    break;
            }
            break;
        }
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            //draw here
            for(int i = 0; i<arr_size; i++){
                ShapeArray[i]->draw(hdc);
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
    ShapeArray[0] = static_cast<Shape*>(new Elipse(Vector2{100,100},Vector2{300,200}));
    arr_size+=1;
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
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    for (int i = 0; i < arr_size; i++) {
        delete ShapeArray[i];
    }
    delete[] ShapeArray; // Use delete[] for dynamic arrays
    return 0;
}