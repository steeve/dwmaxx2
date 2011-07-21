#include <windows.h>
#include <stdio.h>
#include <d3d10_1.h>
#include "globals.h"
#include "hooking.h"
#include "d3d_hooks.h"
#include "D3D10CreateDevice1.h"
#include "ID3D10Device1.h"
#include "rpc_hwnd.h"

HOOK(HRESULT, D3D10CreateDevice1, IDXGIAdapter *pAdapter,
                                  D3D10_DRIVER_TYPE DriverType,
                                  HMODULE Software,
                                  UINT Flags,
                                  D3D10_FEATURE_LEVEL1 HardwareLevel,
                                  UINT SDKVersion,
                                  ID3D10Device1 **ppDevice)
{
    HRESULT result = __D3D10CreateDevice1(pAdapter, DriverType, Software, Flags, HardwareLevel, SDKVersion, ppDevice);

#ifdef _DEBUG
    printf("New D3D device! Patching... 0x%I64X\n", *ppDevice);
#endif
    ApplyD3DHooks(*ppDevice);
#ifdef _DEBUG
    printf("Patching done!\n");
#endif

    return (result);
}
