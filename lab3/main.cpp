#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <windowsx.h> // Required for GET_X_LPARAM and GET_Y_LPARAM
#include <commctrl.h>

// Link Common Controls library
#pragma comment(lib, "comctl32.lib")
#define WIN32_LEAN_AND_MEAN
#include "shapes/shape.h"
#include "shapes/elipse.h"
#include "shapes/vec2.h"
#include "create.h"
#include "paint_window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
    PaintWindow myWindow;

    if (!myWindow.Create(L"Лаба2", WS_OVERLAPPEDWINDOW, 0, 100, 100, 800, 600)) {
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