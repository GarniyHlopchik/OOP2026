#include "lab6_win.h"
#include <winnt.h>
#include <winuser.h>
#include <string>
#include "../helpers.h"
#include "../send_data.h"

#define MENU_FUNCTION 1001

void MainWindow::OnDestroy(){
    TerminateChildProcesses();
    PostQuitMessage(0);
}

bool MainWindow::Create(PCWSTR title, DWORD style, DWORD exStyle,int x, int y,int width, int height,HWND parent, HMENU menu){
    dialogue.parent_ptr = this;

    //building menu
    HMENU hMenu = CreateMenu();
    AppendMenuW(hMenu, MF_STRING, MENU_FUNCTION, L"Виконати");
    if (!Window::Create(title, style, exStyle, x, y, width, height, parent, hMenu)) { return false; }
    return true;
}

extern ChildProcessInfo proc2, proc3;



LRESULT MainWindow::OnCommand(int id, int eventCode, HWND controlHandle){
    if (eventCode == BN_CLICKED) {
        switch(id){
            case MENU_FUNCTION:{
                //StartWork();
                dialogue.Open(L"data input", 200, 200, m_hwnd);
            }
        }
    }
    return Window::OnCommand(id, eventCode, controlHandle);
}
LRESULT MainWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam){
    switch(msg){
        case WM_APP+1:{
            int commandCode = (int)wParam;
            if(commandCode == 101){
                // повідомлення до третього
                HWND obj3 = FindMainWindowForProcess(proc3.pi.dwProcessId);
                SendDataToObject(obj3, &data);
            }
        }
    }
    return Window::HandleMessage(msg, wParam, lParam);
}
