#include "../common/common.h"
#include "../dictionary/dictionary.h"
#include <windows.h>
#include <commctrl.h>
#include <stdbool.h>
#include <wchar.h>

#define MAX_TEXT_LENGTH 4096
#define MAX_DISPLAY_ITEMS 50

HWND editControl, staticControl, listViewControl;
NOTIFYICONDATAW nid;
HFONT hFont;
bool showTranslationFlag = true;

void CreateTrayIcon(HWND hwnd);
void CreateControls(HWND hwnd);
void ToggleWindowVisibility(HWND hwnd);
void HandleListViewSelection(bool showTranslation);
void UpdateListView();

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_CREATE:
        CreateTrayIcon(hwnd);
        CreateControls(hwnd);
        SetFocus(editControl);
        RegisterHotKey(hwnd, HOTKEY_ID, MOD_CONTROL | MOD_ALT, VK_SPACE);
        return 0;

    case WM_SIZE:
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);

        HRGN hRgn = CreateRoundRectRgn(0, 0, width + 1, height + 1, 20, 20);
        SetWindowRgn(hwnd, hRgn, TRUE);
        DeleteObject(hRgn);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_EDIT:
            if (HIWORD(wParam) == EN_CHANGE)
                UpdateListView();
            break;

        case IDM_ESC:
            // PostQuitMessage(0); // NOTE: For debug
            ShowWindow(hwnd, SW_HIDE); // NOTE: For release
            break;

        case IDM_CLEAR:
            SetWindowTextW(editControl, L"");
            UpdateListView();
            break;

        case IDM_TOGGLE_TRANSLATION:
            showTranslationFlag = !showTranslationFlag;
            HandleListViewSelection(showTranslationFlag);
            break;

        case IDM_PREV_ITEM:
        case IDM_NEXT_ITEM:
            int itemCount = ListView_GetItemCount(listViewControl);
            int currentItem = ListView_GetNextItem(listViewControl, -1, LVNI_SELECTED);

            if (LOWORD(wParam) == IDM_PREV_ITEM) {
                currentItem = ((currentItem == 0) ? 0 : (currentItem - 1));
            } else {
                currentItem = ((currentItem == itemCount - 1) ? (itemCount - 1) : (currentItem + 1));
            }

            if (itemCount > 0) {
                ListView_SetItemState(listViewControl, currentItem, LVIS_SELECTED, LVIS_SELECTED);
                ListView_EnsureVisible(listViewControl, currentItem, FALSE);
            }
            HandleListViewSelection(showTranslationFlag);
            break;
        }
        return 0;

    case WM_ACTIVATE:
        if (wParam == WA_INACTIVE) {
            ShowWindow(hwnd, SW_HIDE);
        }
        return 0;

    case WM_HOTKEY:
        if (wParam == HOTKEY_ID) {
            ToggleWindowVisibility(hwnd);
        }
        return 0;

    case WM_NCHITTEST:
        LRESULT hit = DefWindowProc(hwnd, message, wParam, lParam);
        if (hit == HTCLIENT) {
            return HTCAPTION;
        }
        return hit;

    case WM_SETFOCUSTOEDIT:
        SetFocus(editControl);
        return 0;

    case WM_SYSICON:
        if (lParam == WM_LBUTTONDOWN) {
            ToggleWindowVisibility(hwnd);
        } else if (lParam == WM_RBUTTONUP) {
            POINT cursorPos;
            GetCursorPos(&cursorPos);

            HMENU hMenu = CreatePopupMenu();
            AppendMenuW(hMenu, MF_STRING, 1, L"Quit");
            SetForegroundWindow(hwnd);
            int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_LEFTALIGN | TPM_BOTTOMALIGN, cursorPos.x, cursorPos.y, 0, hwnd, NULL);
            DestroyMenu(hMenu);

            if (cmd == 1) {
                PostMessage(hwnd, WM_CLOSE, 0, 0);
            }
        }
        return 0;

    case WM_DESTROY:
        Shell_NotifyIconW(NIM_DELETE, &nid);
        UnregisterHotKey(hwnd, HOTKEY_ID);
        DeleteObject(hFont);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

void CreateTrayIcon(HWND hwnd)
{
    nid.cbSize = sizeof(NOTIFYICONDATAW);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_SYSICON;
    nid.hIcon = LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(1));
    wcscpy_s(nid.szTip, sizeof(nid.szTip) / sizeof(wchar_t), APPNAME);
    Shell_NotifyIconW(NIM_ADD, &nid);
}

void CreateControls(HWND hwnd)
{
    hFont = CreateFontW(
        29,
        0,
        0,
        0,
        FW_NORMAL,
        FALSE,
        FALSE,
        FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS,
        L"Cambria");

    editControl = CreateWindowExW(
        0,
        WC_EDITW,
        NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        26, 27, 746, 36,
        hwnd, (HMENU)ID_EDIT, GetModuleHandleW(NULL), NULL);
    SendMessageW(editControl, WM_SETFONT, (WPARAM)hFont, TRUE);

    listViewControl = CreateWindowExW(
        0,
        WC_LISTVIEWW,
        L"",
        WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_SINGLESEL | LVS_SHOWSELALWAYS,
        26, 81, 200, 432,
        hwnd, (HMENU)ID_LISTVIEW, GetModuleHandleW(NULL), NULL);
    SendMessageW(listViewControl, WM_SETFONT, (WPARAM)hFont, TRUE);
    LVCOLUMN lvc = { 0 };
    lvc.mask = LVCF_WIDTH;
    lvc.cx = 200;
    ListView_InsertColumn(listViewControl, 0, &lvc);

    staticControl = CreateWindowExW(
        0,
        WC_EDITW,
        g_tips,
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | WS_VSCROLL | ES_AUTOVSCROLL,
        26 + 200, 81, 746 - 200, 432,
        hwnd, (HMENU)ID_STATIC, GetModuleHandleW(NULL), NULL);
    SendMessageW(staticControl, WM_SETFONT, (WPARAM)hFont, TRUE);
}

void ToggleWindowVisibility(HWND hwnd)
{
    if (IsWindowVisible(hwnd)) {
        ShowWindow(hwnd, SW_HIDE);
    } else {
        SetWindowTextW(editControl, L"");
        ShowWindow(hwnd, SW_SHOW);
        SetForegroundWindow(hwnd);
        SetFocus(editControl);
    }
}

void UpdateListView()
{
    SendMessageW(listViewControl, LVM_DELETEALLITEMS, 0, 0L);

    wchar_t input[MAX_WORD_LENGTH];
    GetWindowTextW(editControl, input, MAX_WORD_LENGTH);
    if (input[0] == L'\0') {
        SetWindowTextW(staticControl, g_tips);
        return;
    }

    // Find not all (because of MAX_DISPLAY_ITEMS) matched words
    /// TODO:
    ///     1. This is a poor way to improve user's life
    ///     2. Should not be MAX_WORDS, should be real words count
    LVITEMW lvi = { 0 };
    lvi.mask = LVIF_TEXT;
    lvi.iSubItem = 0;

    int itemCount = 0;
    /// first: add items with matching first letter
    for (size_t i = 0; i < MAX_WORDS && itemCount < MAX_DISPLAY_ITEMS; i++) {
        if (towlower(g_allWords[i][0]) == towlower(input[0]) && wcsstr(g_allWords[i], input) != NULL) {
            lvi.iItem = itemCount;
            lvi.pszText = g_allWords[i];
            SendMessageW(listViewControl, LVM_INSERTITEMW, 0, (LPARAM)&lvi);
            itemCount++;
        }
    }

    /// second: add remaining matching items
    for (size_t i = 0; i < MAX_WORDS && itemCount < MAX_DISPLAY_ITEMS; i++) {
        if (wcsstr(g_allWords[i], input) != NULL && towlower(g_allWords[i][0]) != towlower(input[0])) {
            lvi.iItem = itemCount;
            lvi.pszText = g_allWords[i];
            SendMessageW(listViewControl, LVM_INSERTITEMW, 0, (LPARAM)&lvi);
            itemCount++;
        }
    }

    // Search dictionary and display the result
    if (ListView_GetItemCount(listViewControl) > 0) {
        ListView_SetItemState(listViewControl, 0, LVIS_SELECTED, LVIS_SELECTED);
        HandleListViewSelection(showTranslationFlag);
    } else {
        SetWindowTextW(staticControl, L"no matches found");
    }
}

void HandleListViewSelection(bool showTranslation)
{
    int selectedIndex = ListView_GetNextItem(listViewControl, -1, LVNI_SELECTED);
    if (selectedIndex == -1)
        return;
    wchar_t szSelectedWord[MAX_WORD_LENGTH] = { 0 };
    LVITEMW lvi = { 0 };
    lvi.mask = LVIF_TEXT;
    lvi.iItem = selectedIndex;
    lvi.pszText = szSelectedWord;
    lvi.cchTextMax = MAX_WORD_LENGTH;
    SendMessageW(listViewControl, LVM_GETITEMW, 0, (LPARAM)&lvi);

    wchar_t szText[MAX_TEXT_LENGTH] = { 0 };
    SearchDict(szSelectedWord, g_csvData, szText, (size_t)MAX_TEXT_LENGTH, showTranslation);

    // Convert to Windows text format (\n => \r\n)
    wchar_t szFormattedText[MAX_TEXT_LENGTH * 2] = { 0 }; // TODO: MAX_TEXT_LENGTH * 2 ?
    wchar_t* pSrc = szText;
    wchar_t* pDest = szFormattedText;
    while (*pSrc) {
        if (*pSrc == L'\n') {
            *pDest++ = L'\r';
        }
        *pDest++ = *pSrc++;
    }
    *pDest = L'\0';

    SetWindowTextW(staticControl, szFormattedText);
}