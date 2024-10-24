#ifndef COMMON_H
#define COMMON_H

#include <windows.h>
#include <wchar.h>

#define APPNAME L"Dictionary"

#define IDI_APPICON 1
#define IDR_CSVFILE 2
#define IDR_WORDLIST 3

#define WM_SYSICON (WM_USER + 1)
#define WM_SETFOCUSTOEDIT (WM_USER + 2)

#define ID_EDIT 1
#define ID_STATIC 2
#define ID_LISTVIEW 3

#define MSG_TIMER 1

#define HOTKEY_ID 1

#define IDM_ESC 101
#define IDM_CLEAR 102
#define IDM_TOGGLE_TRANSLATION 103
#define IDM_PREV_ITEM 104
#define IDM_NEXT_ITEM 105

#define MAX_CSV_SIZE (10 * 1024 * 1024)
#define MAX_WORDS 30000
#define MAX_WORD_LENGTH 64

#define MAX_TEXT_LENGTH 4096
#define MAX_DISPLAY_ITEMS 50

extern HWND g_hwnd;
extern HHOOK g_hKeyboardHook;
extern HHOOK g_hMouseHook;
extern wchar_t g_csvData[MAX_CSV_SIZE];
extern wchar_t g_allWords[MAX_WORDS][MAX_WORD_LENGTH];
extern const wchar_t* g_tips;

#endif // COMMON_H
