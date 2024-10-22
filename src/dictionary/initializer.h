#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#include <windows.h>

void LoadDictionary(HMODULE hModule, LPCWSTR lpName, LPCWSTR lpType);
void LoadWordList();

#endif // RESOURCE_LOADER_H
