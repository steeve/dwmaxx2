#pragma once
#include <Windows.h>
#include <map>
#include "types.h"

extern HMODULE                              g_hInstance;
extern bool                                 g_isDWM;
extern std::map<ID3D10InputLayout *, int>   g_inputLayouts;
extern std::map<HWND, DwmaxxWindowEntry*>   g_windows;
extern HWND                                 g_rpcHwnd;
extern ID3D10Device1                        *g_pd3dDevice;
extern ID3D10Texture2D                      *g_pixelTexture;
extern ISTOPLEVELWINDOW_PROC                IsTopLevelWindow;
extern HHOOK                                g_wndProcHook;
extern HHOOK                                g_wndProcRetHook;
extern HHOOK                                g_shellHook;
extern HHOOK                                g_cbtHook;
extern PROCESS_INFORMATION                  g_watermarker32;
extern PROCESS_INFORMATION                  g_watermarker64;
extern CRITICAL_SECTION                     g_windowsCS;

extern HMODULE                              g_hEasyHook;
extern LHINSTALLHOOK_PROC                   LhInstallHook;
extern LHUNINSTALLHOOK_PROC                 LhUninstallHook;
extern LHUNINSTALLALLHOOKS_PROC             LhUninstallAllHooks;
extern LHSETEXCLUSIVEACL_PROC               LhSetExclusiveACL;

extern HOOK_TRACE_INFO                      *g_dwmHook;
