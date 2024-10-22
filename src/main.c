#include "common/common.h"
#include "dictionary/initializer.h"
#include "window_procedure/window_procedure.h"
#include <windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(nCmdShow);

    // Create a mutex to ensure only one instance of the application is running
    HANDLE hMutex = CreateMutexW(NULL, TRUE, L"EnZhDictionaryMutex");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        HWND existingHwnd = FindWindowW(NULL, L"En-Zh Dictionary");
        if (existingHwnd) {
            ShowWindow(existingHwnd, SW_SHOW);
            SetForegroundWindow(existingHwnd);
            SendMessageW(existingHwnd, WM_SETFOCUSTOEDIT, 0, 0);
        }
        return 0;
    }

    // Set DPI awareness for better scaling on high DPI displays (Windows 10, v1607)
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    // Set keymap
    HACCEL hAccel;
    ACCEL accel[] = {
        { FVIRTKEY, VK_ESCAPE, IDM_ESC },
        { FVIRTKEY | FCONTROL, L'L', IDM_CLEAR },
        { FVIRTKEY, VK_TAB, IDM_TOGGLE_TRANSLATION },
        { FVIRTKEY | FCONTROL, L'P', IDM_PREV_ITEM },
        { FVIRTKEY | FCONTROL, L'N', IDM_NEXT_ITEM }
    };
    hAccel = CreateAcceleratorTableW(accel, 5);

    // Register the window class
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_APPICON));
    wc.hbrBackground = CreateSolidBrush(RGB(252, 252, 252));
    wc.lpszClassName = APPNAME;
    RegisterClassW(&wc);

    // Calculate the window position and size based task bar
    RECT workArea;
    SystemParametersInfoW(SPI_GETWORKAREA, 0, &workArea, 0);
    int windowWidth = 800;
    int windowHeight = 540;
    int xPos = workArea.right - windowWidth - 10;
    int yPos = workArea.bottom - windowHeight - 10;

    // Create the window
    HWND hwnd = CreateWindowExW(
        WS_EX_TOOLWINDOW,
        wc.lpszClassName,
        L"En-Zh Dictionary",
        WS_POPUP | WS_VISIBLE,
        xPos, yPos,
        windowWidth, windowHeight,
        NULL, NULL, hInstance, NULL);

    // Load dictionary resource
    LoadDictionary(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDR_CSVFILE), (LPCWSTR)RT_RCDATA);
    LoadWordList();

    // Run the message loop.
    MSG msg = { 0 };
    while (GetMessageW(&msg, NULL, 0, 0)) {
        if (!TranslateAcceleratorW(hwnd, hAccel, &msg)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    // Clean
    DestroyAcceleratorTable(hAccel);
    CloseHandle(hMutex);
    return 0;
}
