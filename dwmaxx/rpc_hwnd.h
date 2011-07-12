#pragma once
#include <Windows.h>

#define DWMAXX_RPC_WINDOW_CLASS  ("DwmaxxRPCEndpoint")
#define DWMAXX_GET_SHARED_HANDLE (WM_APP + 1)
#define DWMAXX_REMOVE_WINDOW     (WM_APP + 2)
#define DWMAXX_UNLOAD            (WM_APP + 3)

LRESULT CALLBACK    RpcWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI        CreateRpcWindow(LPVOID lpParameter);
