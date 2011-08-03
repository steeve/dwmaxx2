#include <windows.h>
#include <stdio.h>
#include <dwmapi.h>
#include "globals.h"
#include "hooking.h"
#include "dwmaxx_private.h"
#include "rpc_hwnd.h"
#include "DwmExtendFrameIntoClientArea.h"

HOOK(HRESULT, DwmExtendFrameIntoClientArea, HWND hWnd,
                                            const MARGINS *pMarInset)
{
    if (pMarInset != NULL)
    {
        PostMessage(DwmaxxRpcWindow(), DWMAXX_SETAREATOPLEFT, (WPARAM)hWnd, (((pMarInset->cyTopHeight & 0xFFFF) << 16)
                                                                            | (pMarInset->cxLeftWidth & 0xFFFF)));
        PostMessage(DwmaxxRpcWindow(), DWMAXX_SETAREABOTTOMRIGHT, (WPARAM)hWnd, (((pMarInset->cyBottomHeight & 0xFFFF) << 16)
                                                                                | (pMarInset->cxRightWidth & 0xFFFF)));
    }

    return (DwmExtendFrameIntoClientArea(hWnd, pMarInset));
}
