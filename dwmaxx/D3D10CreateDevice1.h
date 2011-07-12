#pragma once

#include <windows.h>
#include <stdio.h>
#include <d3d10_1.h>
#include "hooking.h"

DEFINE_HOOK(HRESULT, D3D10CreateDevice1,
    IDXGIAdapter *pAdapter,
    D3D10_DRIVER_TYPE DriverType,
    HMODULE Software,
    UINT Flags,
    D3D10_FEATURE_LEVEL1 HardwareLevel,
    UINT SDKVersion,
    ID3D10Device1 **ppDevice
);
