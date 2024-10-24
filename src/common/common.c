#include "common.h"
#include <windows.h>
#include <wchar.h>

wchar_t g_csvData[MAX_CSV_SIZE] = { 0 };
wchar_t g_allWords[MAX_WORDS][MAX_WORD_LENGTH] = { 0 };
const wchar_t* g_tips = L" Tips: \r\n"
                        L" - I will be hided to tray when loses focus.\r\n"
                        L" - it is possible to move me.\r\n"
                        L" - <ctrl-alt-space> to toggle or hide me.\r\n"
                        L" - <esc> to hide me.\r\n"
                        L" - <tab> to switch Chinese & English mode.\r\n"
                        L" - <ctrl-l> to clear the input.\r\n"
                        L" - <ctrl+p/n> to select previous/next word.";
