#pragma once
#include "windows.h"
#include <string>
#include "send_data.h"

struct ChildProcessInfo {
    PROCESS_INFORMATION pi;
    bool isRunning = false;

    void Close();
};

struct WindowSearchData {
    DWORD processId;
    HWND foundHwnd;
};

void SendDataToObject(HWND hwndTarget,const GenData* cmd);
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
HWND FindMainWindowForProcess(DWORD processId);
void ArrangeObjectWindows(DWORD pid2, DWORD pid3);
void TerminateChildProcesses();
bool LaunchChildProcess(const std::wstring& exeName, const std::wstring& arguments, ChildProcessInfo& outProc);
