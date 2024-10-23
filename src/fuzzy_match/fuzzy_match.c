#include "../common/common.h"
#include <wchar.h>

/// TODO:
///     1. This is a poor way to improve user's life
///     2. Should not be MAX_WORDS, should be real words count
void fuzzy_match(wchar_t* search, wchar_t all_words[MAX_WORDS][MAX_WORD_LENGTH], int idx[MAX_WORDS]) {
    // Init idx to -1
    for (int i = 0; i < MAX_WORDS; i++)
        idx[i] = -1;

    // Assign sorted matched words index to idx
    int j = 0, k = MAX_WORDS - 1;
    for (int i = 0; i < MAX_WORDS; i++) {
        if (wcsstr(all_words[i], search) != NULL) {
            /// add items with matching first letter to head
            if (towlower(all_words[i][0]) == towlower(search[0]))
                idx[j++] = i;
            /// add remaining matching items to tail
            else
                idx[k--] = i;
        }
    }
    /// move tail to next head
    while (++k < MAX_WORDS) {
        idx[j++] = idx[k];
        idx[k] = -1;
    }

    // Real indexes should not over MAX_DISPLAY_ITEMS
    for (int i = MAX_DISPLAY_ITEMS; idx[i] != -1; i++)
        idx[i] = -1;
}
