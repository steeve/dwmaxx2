#pragma once
#include <Windows.h>

#define DWMAXX_RPC_WINDOW_CLASS     ("DwmaxxRPCEndpoint")
#define DWMAXX_GET_SHARED_HANDLE    (WM_APP + 1)
#define DWMAXX_REMOVE_WINDOW        (WM_APP + 2)
#define DWMAXX_UNLOAD               (WM_APP + 3)
#define DWMAXX_GETAREATOPLEFT       (WM_APP + 4)
#define DWMAXX_GETAREABOTTOMRIGHT   (WM_APP + 5)
#define DWMAXX_SETAREATOPLEFT       (WM_APP + 6)
#define DWMAXX_SETAREABOTTOMRIGHT   (WM_APP + 7)

LRESULT CALLBACK    RpcWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI        CreateRpcWindow(LPVOID lpParameter);
