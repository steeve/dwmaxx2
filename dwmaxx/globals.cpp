#include <Windows.h>
#include <map>
#include <list>
#include "types.h"

HMODULE                             g_hInstance = NULL;
bool                                g_isDWM = false;
std::map<ID3D10InputLayout *, int>  g_inputLayouts;
std::map<HWND, DwmaxxWindowEntry*>  g_windows;
HWND                                g_rpcHwnd = NULL;
std::list<ID3D10Device1 *>          g_pd3dDevices;
ID3D10Texture2D                     *g_pixelTexture = NULL;
ISTOPLEVELWINDOW_PROC               IsTopLevelWindow = NULL;
PROCESS_INFORMATION                 g_watermarker32 = { NULL, NULL, NULL, NULL };
PROCESS_INFORMATION                 g_watermarker64 = { NULL, NULL, NULL, NULL };
CRITICAL_SECTION                    g_windowsCS;

HMODULE                             g_hEasyHook = NULL;
LHINSTALLHOOK_PROC                  LhInstallHook = NULL;
LHUNINSTALLHOOK_PROC                LhUninstallHook = NULL;
LHUNINSTALLALLHOOKS_PROC            LhUninstallAllHooks = NULL;
LHSETEXCLUSIVEACL_PROC              LhSetExclusiveACL = NULL;

HOOK_TRACE_INFO                     *g_dwmHook = NULL;

#pragma data_seg(".SHRDATA")
HHOOK                               g_wndProcHook = NULL;
HHOOK                               g_wndProcRetHook = NULL;
HHOOK                               g_shellHook = NULL;
HHOOK                               g_cbtHook = NULL;
#pragma data_seg()
