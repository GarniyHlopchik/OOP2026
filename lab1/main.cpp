#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>

#define MENU_FILE    1
#define MENU_WORK_1  2
#define MENU_WORK_2  3
#define MENU_ABOUT   4

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            switch (wmId) {
                case MENU_WORK_1:
                    break;
                case MENU_WORK_2:
                    break;
                case MENU_ABOUT:
                    MessageBox(hwnd, L"Win32 Menu Example v1.0", L"About", MB_OK | MB_ICONINFORMATION);
                    break;
            }
            break;
        }
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
    //win class
    const wchar_t className[] = L"LAB1";
    WNDCLASS winClass{
        .lpfnWndProc = WndProc,
        .hInstance = hInstance,
        .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
        .lpszClassName = className
    };
    RegisterClass(&winClass);
    //building menu
    HMENU hMenu = CreateMenu();
    AppendMenuW(hMenu, MF_STRING, MENU_FILE, L"File");
    HMENU hActionMenu = CreatePopupMenu();
    AppendMenuW(hActionMenu, MF_STRING, MENU_WORK_1, L"Work1");
    AppendMenuW(hActionMenu, MF_STRING, MENU_WORK_2, L"Work2");
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hActionMenu, L"Actions");
    AppendMenuW(hMenu, MF_STRING, MENU_ABOUT, L"About");
    //building window
    HWND hwnd = CreateWindowEx(
        0,
        className,
        L"Lab1 main",
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
    return 0;
}