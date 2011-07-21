#include <Windows.h>
#include <stdio.h>
#include <map>
#include "rpc_hwnd.h"
#include "types.h"
#include "globals.h"
#include "dwmaxx_private.h"

DwmaxxWindowEntry   *EnsureWindowEntry(HWND hWnd)
{
    DwmaxxWindowEntry *entry = g_windows[hWnd];
    if (entry == NULL)
    {
        entry = new DwmaxxWindowEntry;
        g_windows[hWnd] = entry;
    }
    return (entry);
}

LRESULT CALLBACK RpcWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case DWMAXX_GET_SHARED_HANDLE:
        //EnterCriticalSection(&g_windowsCS);
        if (g_windows.find((HWND)wParam) != g_windows.end())
        {
            DwmaxxWindowEntry *entry = g_windows[(HWND)wParam];
            if (entry != NULL)
            {
                // improves performance when resolving a lot of windows
                if (lParam != NULL)
                    PostMessage((HWND)lParam,
                                DWMAXX_GET_SHARED_HANDLE,
                                wParam,
                                (LPARAM)entry->sharedTextureHandle);
                else
                    ReplyMessage((LRESULT)entry->sharedTextureHandle);
            }
        }
        //LeaveCriticalSection(&g_windowsCS);
        break;
    case DWMAXX_REMOVE_WINDOW:
        //EnterCriticalSection(&g_windowsCS);
        if (g_windows.find((HWND)wParam) != g_windows.end())
        {
            DwmaxxWindowEntry *entry = g_windows[(HWND)wParam];
            if (entry != NULL)
                delete entry;
            g_windows.erase((HWND)wParam);
#ifdef _DEBUG
            printf("REMOVED: hWnd=0x%08I64x\n", wParam);
#endif
        }
        //LeaveCriticalSection(&g_windowsCS);
        break;
    case DWMAXX_GETAREATOPLEFT:
        //EnterCriticalSection(&g_windowsCS);
        if (g_windows.find((HWND)wParam) != g_windows.end())
        {
            DwmaxxWindowEntry *entry = g_windows[(HWND)wParam];
            if (entry != NULL)
                ReplyMessage((LRESULT)(((entry->dwmMargins.cyTopHeight & 0xFFFF) << 16)
                                      | (entry->dwmMargins.cxLeftWidth & 0xFFFF)));
        }
        //LeaveCriticalSection(&g_windowsCS);
        break;
    case DWMAXX_GETAREABOTTOMRIGHT:
        //EnterCriticalSection(&g_windowsCS);
        if (g_windows.find((HWND)wParam) != g_windows.end())
        {
            DwmaxxWindowEntry *entry = g_windows[(HWND)wParam];
            if (entry != NULL)
                ReplyMessage((LRESULT)(((entry->dwmMargins.cyBottomHeight & 0xFFFF) << 16)
                                      | (entry->dwmMargins.cxRightWidth & 0xFFFF)));
        }
        //LeaveCriticalSection(&g_windowsCS);
        break;
    case DWMAXX_SETAREATOPLEFT:
        {
            //EnterCriticalSection(&g_windowsCS);
            DwmaxxWindowEntry *entry = EnsureWindowEntry((HWND)wParam);
            entry->dwmMargins.cyTopHeight = (((DWORD)lParam >> 16) & 0xFFFF);
            entry->dwmMargins.cxLeftWidth = ((DWORD)lParam & 0xFFFF);
            //LeaveCriticalSection(&g_windowsCS);
#ifdef _DEBUG
            //printf("MARGINS: hWnd=0x%08I64x, Top=%u, Left=%u\n", wParam, entry->dwmMargins.cyTopHeight, entry->dwmMargins.cxLeftWidth);
#endif
        }
        break;
    case DWMAXX_SETAREABOTTOMRIGHT:
        {
            //EnterCriticalSection(&g_windowsCS);
            DwmaxxWindowEntry *entry = EnsureWindowEntry((HWND)wParam);
            entry->dwmMargins.cyBottomHeight = (((DWORD)lParam >> 16) & 0xFFFF);
            entry->dwmMargins.cxRightWidth = ((DWORD)lParam & 0xFFFF);
            //LeaveCriticalSection(&g_windowsCS);
#ifdef _DEBUG
            //printf("MARGINS: hWnd=0x%08I64x, Bottom=%u, Right=%u\n", wParam, entry->dwmMargins.cyBottomHeight, entry->dwmMargins.cxRightWidth);
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
