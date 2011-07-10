#pragma once

#include <Windows.h>

#define DWMAXX_RPC_WINDOW_CLASS  ("DwmaxxRPCEndpoint")
#define DWMAXX_GET_SHARED_HANDLE (WM_APP + 1)
#define DWMAXX_REMOVE_WINDOW     (WM_APP + 2)

DWORD WINAPI CreateRpcWindow(LPVOID lpParameter);
