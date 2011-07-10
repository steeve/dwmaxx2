#include <Windows.h>

HINSTANCE   g_hInstance = NULL;

#pragma data_seg(".SHRDATA")
HHOOK       g_wndProcHook = NULL;
HHOOK       g_wndProcRetHook = NULL;
HHOOK       g_shellHook = NULL;
#pragma data_seg()
