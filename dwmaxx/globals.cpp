#include <Windows.h>
#include <map>
#include "types.h"

HMODULE                             g_hInstance = NULL;
bool                                g_isDWM = false;
std::map<ID3D10InputLayout *, int>  g_inputLayouts;
std::map<HWND, HANDLE>              g_sharedHandles;
HWND                                g_rpcHwnd = NULL;
ID3D10Texture2D                     *g_pixelTexture = NULL;
ISTOPLEVELWINDOW_PROC               IsTopLevelWindow = (ISTOPLEVELWINDOW_PROC)GetProcAddress(GetModuleHandle("user32.dll"), "IsTopLevelWindow");

#pragma data_seg(".SHRDATA")
HHOOK                               g_wndProcHook = NULL;
HHOOK                               g_wndProcRetHook = NULL;
HHOOK                               g_shellHook = NULL;
#pragma data_seg()
