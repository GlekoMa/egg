#ifndef KEYBOAD_MOUSE_PROCEDURE_H
#define KEYBOAD_MOUSE_PROCEDURE_H

#include <windows.h>

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);

#endif // KEYBOAD_MOUSE_PROCEDURE_H
