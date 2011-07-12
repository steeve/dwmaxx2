#pragma once

#include <Windows.h>
#include "types.h"

extern HINSTANCE                g_hInstance;
extern HHOOK                    g_wndProcHook;
extern HHOOK                    g_wndProcRetHook;
extern HHOOK                    g_shellHook;
extern ISTOPLEVELWINDOW_PROC    IsTopLevelWindow;