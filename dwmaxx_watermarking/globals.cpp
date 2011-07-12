#include <Windows.h>
#include "types.h"

HINSTANCE               g_hInstance = NULL;
ISTOPLEVELWINDOW_PROC   IsTopLevelWindow = (ISTOPLEVELWINDOW_PROC)GetProcAddress(GetModuleHandle("user32.dll"), "IsTopLevelWindow");

#pragma data_seg(".SHRDATA")
HHOOK       g_wndProcHook = NULL;
HHOOK       g_wndProcRetHook = NULL;
HHOOK       g_shellHook = NULL;
#pragma data_seg()
