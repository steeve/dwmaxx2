#pragma once
#include <Windows.h>
#include "globals.h"

BOOL                IsWindowWatermarkable(HWND hWnd);
LRESULT CALLBACK    WndProcProlog(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK    WndProcEpilog(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK    ShellProcProlog(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK    CBTProc(int nCode, WPARAM wParam, LPARAM lParam);

void                DwmaxxInstallHooks();
void                DwmaxxRemoveHooks();
