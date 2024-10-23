#ifndef FUZZY_MATCH_H
#define FUZZY_MATCH_H

#include "../common/common.h"
#include <wchar.h>

void fuzzy_match(wchar_t* search, wchar_t all_words[MAX_WORDS][MAX_WORD_LENGTH], int idx[MAX_WORDS]);

#endif // FUZZY_MATCH_H
