#include "windows.h"
#include <string>
#include "helpers.h"
#include "send_data.h"
#include <iostream>

void ChildProcessInfo::Close() {
    if (isRunning) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        isRunning = false;
    }
}


extern ChildProcessInfo proc2, proc3;

void SendDataToObject(HWND hwndTarget, const GenData* cmd) {
    COPYDATASTRUCT cds;
    cds.dwData = 1;
    cds.cbData = sizeof(GenData);
    cds.lpData = (void*)cmd;

    wchar_t cls[256] = L"(null)";
    if (hwndTarget) GetClassNameW(hwndTarget, cls, 256);

    SetLastError(0);
    LRESULT r = SendMessageW(hwndTarget, WM_COPYDATA, (WPARAM)nullptr, (LPARAM)&cds);
    DWORD err = GetLastError();

    wchar_t info[512];
    swprintf_s(info, L"hwnd=%p\nclass=%s\nresult=%lld\nerror=%lu",
               (void*)hwndTarget, cls, (long long)r, err);

}
// Callback-функція для пошуку головного вікна за PID
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    WindowSearchData* data = (WindowSearchData*)lParam;
    DWORD windowPid = 0;
    GetWindowThreadProcessId(hwnd, &windowPid);

    // Перевіряємо PID, чи це головне вікно (без власника) і чи воно видиме
    if (windowPid == data->processId && GetWindow(hwnd, GW_OWNER) == nullptr && IsWindowVisible(hwnd)) {
        data->foundHwnd = hwnd;
        return FALSE; // Зупиняємо перебір, бо вікно знайдено
    }
    return TRUE; // Продовжуємо пошук
}

HWND FindMainWindowForProcess(DWORD processId) {
    WindowSearchData data;
    data.processId = processId;
    data.foundHwnd = nullptr;

    EnumWindows(EnumWindowsProc, (LPARAM)&data);

    return data.foundHwnd;
}

// Функція автоматичного позиціонування
void ArrangeObjectWindows(DWORD pid2, DWORD pid3) {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Іноді дочірні програми запускаються за частку секунди і їхнє вікно
    // ще не встигає з'явитися в системі одразу після CreateProcess.
    // Робимо невелику перевірку-цикл очікування (наприклад, до ~0.5 сек) з паузами:
    HWND hwnd2 = nullptr;
    HWND hwnd3 = nullptr;

    for (int i = 0; i < 10; ++i) {
        if (!hwnd2) hwnd2 = FindMainWindowForProcess(pid2);
        if (!hwnd3) hwnd3 = FindMainWindowForProcess(pid3);
        if (hwnd2 && hwnd3) break;
        Sleep(50); // Чекаємо 50 мс перед наступною спробою
    }

    // Розташовуємо Object2 ліворуч, а Object3 праворуч
    if (hwnd2 && IsWindow(hwnd2)) {
        // Параметри: HWND, hwndInsertAfter, x, y, cx, cy, flags
        SetWindowPos(hwnd2, HWND_TOP, 0, 50, screenWidth / 2 - 10, screenHeight - 150, SWP_SHOWWINDOW);
    }

    if (hwnd3 && IsWindow(hwnd3)) {
        SetWindowPos(hwnd3, HWND_TOP, screenWidth / 2 + 10, 50, screenWidth / 2 - 10, screenHeight - 150, SWP_SHOWWINDOW);
    }
}

void TerminateChildProcesses() {
    auto closeChild = [](ChildProcessInfo& proc) {
        if (!proc.isRunning) return;

        HWND hwnd = FindMainWindowForProcess(proc.pi.dwProcessId);
        if (hwnd && IsWindow(hwnd)) {
            // 1. Просимо вікно закритися м'яко
            PostMessageW(hwnd, WM_CLOSE, 0, 0);

            // 2. Чекаємо до 500 мс, поки процес реально завершиться
            DWORD waitResult = WaitForSingleObject(proc.pi.hProcess, 500);

            // 3. Якщо процес завис і не закрився — примусово завершуємо
            if (waitResult == WAIT_TIMEOUT) {
                TerminateProcess(proc.pi.hProcess, 0);
                WaitForSingleObject(proc.pi.hProcess, 100); // Чекаємо ще трохи на смерть процесу
            }
        } else {
            // Якщо вікна вже немає, але процес живий — рубаємо примусово
            TerminateProcess(proc.pi.hProcess, 0);
        }

        // 4. Закриваємо дескриптори тільки після повного завершення процесу
        proc.Close();
    };

    closeChild(proc2);
    closeChild(proc3);
}

bool LaunchChildProcess(const std::wstring& exeName, const std::wstring& arguments, ChildProcessInfo& outProc) {
    // Перевірка (вимога 4): якщо процес вже був викликаний, не плодимо копії або закриваємо/переісовуємо
    if (outProc.isRunning) {
        // Перевіряємо, чи процес ще живий
        DWORD exitCode;
        if (GetExitCodeProcess(outProc.pi.hProcess, &exitCode) && exitCode == STILL_ACTIVE) {
            return true; // Вже працює
        } else {
            outProc.Close(); // Завершився раніше — очищаємо дескриптори
        }
    }

    STARTUPINFOW si = { sizeof(si) };
    // Можна налаштувати показ вікна, наприклад, SW_SHOW
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;

    // CreateProcessW вимагає неконстантного рядка для командного рядка
    std::wstring cmdLine = exeName + L" " + arguments;
    wchar_t* cmdLineMutable = const_cast<wchar_t*>(cmdLine.c_str());

    ZeroMemory(&outProc.pi, sizeof(outProc.pi));

    BOOL success = CreateProcessW(
        nullptr,             // Шлях до модуля (можна взяти з argv або задати явно)
        cmdLineMutable,      // Командний рядок
        nullptr,             // Process security attributes
        nullptr,             // Thread security attributes
        FALSE,               // Inherit handles
        0,                   // Creation flags
        nullptr,             // Environment block
        nullptr,             // Current directory
        &si,                 // Startup info
        &outProc.pi          // Process information
    );

    if (success) {
        outProc.isRunning = true;
        return true;
    }
    return false;
}
