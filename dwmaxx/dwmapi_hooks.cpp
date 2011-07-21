#include <Windows.h>
#include "..\\easyhook\\easyhook.h"
#include "dwmapi_hooks.h"
#include "DwmExtendFrameIntoClientArea.h"
#include <dwmapi.h>

HOOK_TRACE_INFO *info = new HOOK_TRACE_INFO();

void    dwmapi_InstallHooks()
{
    DWORD allThreads = 0;
    LhInstallHook(DwmExtendFrameIntoClientArea, My_DwmExtendFrameIntoClientArea, NULL, info);
    LhSetExclusiveACL(&allThreads, 0, info);
}

void    dwmapi_RemoveHooks()
{
    if (info != NULL)
    {
        LhUninstallHook(info);
        delete info;
    }
}
