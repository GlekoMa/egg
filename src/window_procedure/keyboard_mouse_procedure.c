#include "../common/common.h"
#include <windows.h>
#include <stdbool.h>

bool inCtrlContext = false;
DWORD lastClickTime = 0;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* pKeyBoard = (KBDLLHOOKSTRUCT*)lParam;
        if (pKeyBoard->vkCode == VK_LCONTROL || pKeyBoard->vkCode == VK_RCONTROL) {
            if (wParam == WM_KEYDOWN) {
                inCtrlContext = true;
            } else if (wParam == WM_KEYUP) {
                inCtrlContext = false;
            }
        }
    }
    return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION) {
        if (wParam == WM_LBUTTONDOWN) {
            DWORD currentTime = GetTickCount();
            if (currentTime - lastClickTime < GetDoubleClickTime() && inCtrlContext == true)
                SetTimer(g_hwnd, MSG_TIMER, 300, NULL);
            lastClickTime = currentTime;
        }
    }
    return CallNextHookEx(g_hMouseHook, nCode, wParam, lParam);
}
