#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>

typedef struct {
    wchar_t* word;
    wchar_t* pronunciation;
    wchar_t* partofspeech;
    wchar_t* definition;
    wchar_t* translation;
} Word;

void SearchDict(wchar_t* szSearch, const wchar_t* csvData, wchar_t* szText, size_t textSize, bool ShowTranslation)
{
    // As we frequently alloc & free memory, so maybe should check if the memory block is reused?
    wchar_t* csvCopy = _wcsdup(csvData);
    int matchCount = 0;
    wchar_t* line = csvCopy;

    // Loop through each line of the CSV data
    while (line != NULL && matchCount < 15) { // TODO: < 15? God! `go` have 200+ def! can't indulge it!
        wchar_t* nextLine = wcschr(line, L'\n');
        if (nextLine != NULL) {
            *nextLine = L'\0';
            nextLine++;
        }

        // Array to hold fields split by comma
        wchar_t* fields[5] = { 0 };
        int fieldIndex = 0;
        wchar_t* token = line;

        // Parse the line to extract fields
        bool inQuotes = false; // to handle e.g. `word,pron,pos,"def-part, def-part2",tran`
        while (*token && fieldIndex < 5) {
            if (*token == L'"') {
                inQuotes = !inQuotes;
                if (inQuotes)
                    line++;
            } else if (*token == L',' && !inQuotes) {
                if (*(token - 1) == L'"')
                    *(token - 1) = L'\0';
                else
                    *token = L'\0';
                fields[fieldIndex++] = line;
                line = token + 1;
            }
            token++;
        }
        fields[fieldIndex] = line;

        // Create a Word instance and assign the fields
        Word word = { NULL, NULL, NULL, NULL, NULL };
        word.word = fields[0];
        word.pronunciation = fields[1];
        word.definition = fields[3];
        word.translation = fields[4];

        // If the word been searched matches the word of current line, format and save it to szText
        if (word.word != NULL && _wcsicmp(word.word, szSearch) == 0) {
            // Store word and pronunciation only on first match
            if (matchCount == 0)
                swprintf_s(szText + wcslen(szText), textSize - wcslen(szText), L" %s /%s/\n", word.word, word.pronunciation);

            // Add translation/definition from current line to the tail of szText
            if (ShowTranslation) {
                swprintf_s(szText + wcslen(szText), textSize - wcslen(szText), L" %d. %s\n", matchCount + 1, word.translation);
            } else {
                swprintf_s(szText + wcslen(szText), textSize - wcslen(szText), L" %d. %s\n", matchCount + 1, word.definition);
            }
            matchCount++;
        }

        line = nextLine;
    }

    if (matchCount == 0) {
        wcscpy_s(szText, textSize, L" no matches found");
    }
    free(csvCopy);
}
