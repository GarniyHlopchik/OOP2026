#include "window.h"
#include "obj2_win.h"
OBJ2Window myWindow;



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
    HWND parent = FindWindowW(L"LAB6MAIN", nullptr);

    if (!myWindow.Create(L"Об'єкт 2", WS_OVERLAPPEDWINDOW, 0, 300, 100, 800, 600)) {
        return 0;
    }
    myWindow.parent = parent;

    myWindow.Show(nCmdShow);

    // Цикл обробки повідомлень
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
