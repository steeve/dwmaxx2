#include <Windows.h>
#include <stdio.h>
#include <map>
#include "rpc_hwnd.h"
#include "types.h"
#include "globals.h"
#include "dwmaxx_private.h"

LRESULT CALLBACK RpcWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case DWMAXX_GET_SHARED_HANDLE:
        // improves performance when resolving a lot of windows
        if (lParam != NULL)
            PostMessage((HWND)lParam,
                        DWMAXX_GET_SHARED_HANDLE,
                        wParam,
                        (LPARAM)g_sharedHandles[(HWND)wParam]);
        else
            ReplyMessage((LRESULT)g_sharedHandles[(HWND)wParam]);
        break;
    case DWMAXX_REMOVE_WINDOW:
        if (g_sharedHandles.find((HWND)wParam) != g_sharedHandles.end())
        {
            g_sharedHandles.erase((HWND)wParam);
#ifdef _DEBUG
            printf("REMOVED: hWnd=0x%08I64x\n", wParam);
#endif
        }
        break;
    case DWMAXX_UNLOAD:
        DwmaxxTerminate();
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

DWORD WINAPI CreateRpcWindow(LPVOID lpParameter)
{
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.lpfnWndProc = (WNDPROC)RpcWndProc;
    wc.lpszClassName = DWMAXX_RPC_WINDOW_CLASS;
    RegisterClass(&wc);

    g_rpcHwnd = CreateWindowEx(NULL, DWMAXX_RPC_WINDOW_CLASS, NULL, NULL, 0, 0, 1, 1, HWND_MESSAGE, NULL, NULL, NULL);

    MSG  msg;
    while (GetMessage(&msg, NULL, 0, 0))
        DispatchMessage(&msg);

    return ((int)msg.wParam);
}
