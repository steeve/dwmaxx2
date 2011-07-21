#pragma once
#include <Windows.h>
#include "DwmExtendFrameIntoClientArea.h"
#include "..\\easyhook\\easyhook.h"

//#ifdef _WIN64
//#pragma comment (lib, "..\\easyhook\\EasyHook64.lib")
//#else
//#pragma comment (lib, "..\\easyhook\\EasyHook32.lib")
//#endif


void    dwmapi_InstallHooks();
void    dwmapi_RemoveHooks();
