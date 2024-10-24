#ifndef FUZZY_MATCH_H
#define FUZZY_MATCH_H

#include "../common/common.h"
#include <wchar.h>

void FuzzyMatch(wchar_t* search, wchar_t (*all_words)[MAX_WORD_LENGTH], int* idx);

#endif // FUZZY_MATCH_H
