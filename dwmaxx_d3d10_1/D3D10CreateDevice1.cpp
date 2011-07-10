#pragma once

#include <windows.h>
#include <stdio.h>

#define CINTERFACE
#include <d3d10_1.h>
#include "globals.h"
#include "hooking.h"
#include "D3D10CreateDevice1.h"
#include "ID3D10Device1.h"
#include "rpc_hwnd.h"
//#include "..\\dwmex_watermarker\\watermarker.h"
//#pragma comment(lib, "C:\\Dev\\lumier\\d3d10_1\\x64\\Debug\\dwmex_watermarker.lib")

HOOK(HRESULT, D3D10CreateDevice1, IDXGIAdapter *pAdapter,
                                  D3D10_DRIVER_TYPE DriverType,
                                  HMODULE Software,
                                  UINT Flags,
                                  D3D10_FEATURE_LEVEL1 HardwareLevel,
                                  UINT SDKVersion,
                                  ID3D10Device1 **ppDevice)
{
    if (__D3D10CreateDevice1 == NULL)
        __D3D10CreateDevice1 = (D3D10CreateDevice1_PROC)GetProcAddress(g_hD3D10, "D3D10CreateDevice1");

    HRESULT result = __D3D10CreateDevice1(pAdapter, DriverType, Software, Flags, HardwareLevel, SDKVersion, ppDevice);

    if (g_isDWM == true)
    {
        DX_METHOD_HOOK(*ppDevice, ID3D10Device1, Draw);
        DX_METHOD_HOOK(*ppDevice, ID3D10Device1, CreateInputLayout);
        DX_METHOD_HOOK(*ppDevice, ID3D10Device1, CreateTexture2D);

        if (g_rpcHwnd == NULL)
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CreateRpcWindow, NULL, 0, NULL);
    }

    return (result);
}
