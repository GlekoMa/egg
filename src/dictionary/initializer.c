#include "../common/common.h"
#include <windows.h>
#include <wchar.h>

void LoadDictionary()
{
    HINSTANCE hInstance = GetModuleHandleW(NULL);
    HRSRC hRes = FindResourceW(hInstance, MAKEINTRESOURCEW(IDR_CSVFILE), (LPCWSTR)RT_RCDATA);
    HGLOBAL hData = LoadResource(hInstance, hRes);
    LPVOID lpRes = LockResource(hData);

    DWORD dwSize = SizeofResource(hInstance, hRes);
    memcpy(g_csvData, lpRes, dwSize);
    // NOTE: Somehow, there will be a space at the first character, so we omit it
    g_csvData[0] = '\n';
    g_csvData[dwSize / sizeof(wchar_t)] = L'\0';
}

void LoadWordList()
{
    HINSTANCE hInstance = GetModuleHandleW(NULL);
    HRSRC hRes = FindResourceW(hInstance, MAKEINTRESOURCEW(IDR_WORDLIST), (LPCWSTR)RT_RCDATA);
    HGLOBAL hData = LoadResource(hInstance, hRes);
    DWORD dwSize = SizeofResource(hInstance, hRes);
    const wchar_t* data = (const wchar_t*)LockResource(hData);

    // NOTE: Somehow, there will be a space at the first character, so we init `start` to `data + 1`
    const wchar_t* end = data + (dwSize / sizeof(wchar_t));
    const wchar_t* start = data + 1;

    int count = 0;
    while (start < end && count < MAX_WORDS) {
        const wchar_t* lineEnd = wcschr(start, L'\n');
        if (!lineEnd)
            lineEnd = end;

        size_t lineLength = lineEnd - start;
        if (lineLength > 0 && start[lineLength - 1] == L'\r') {
            lineLength--;
        }

        if (lineLength > 0 && lineLength < MAX_WORD_LENGTH) {
            wcsncpy_s(g_allWords[count], MAX_WORD_LENGTH, start, lineLength);
            g_allWords[count][lineLength] = L'\0';
            count++;
        }

        start = lineEnd + 1;
    }
}
