#include <string>
#include "helpers.h"
#include "lab6_win.h"

MainWindow myWindow;

ChildProcessInfo proc2, proc3;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {


    if (!myWindow.Create(L"Лаба6", WS_OVERLAPPEDWINDOW, 0, 100, 100, 800, 600)) {
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
