#include "window.h"
#include "obj3/obj3_win.h"
OBJ3Window myWindow;



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {


    if (!myWindow.Create(L"Об'єкт 3", WS_OVERLAPPEDWINDOW, 0, 500, 100, 800, 600)) {
        return 0;
    }

    myWindow.Show(nCmdShow);

    // Цикл обробки повідомлень
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
