#include <Windows.h>
#define CINTERFACE
#include "d3d_hooks.h"
#include "globals.h"
#include "hooking.h"
#include "D3D10CreateDevice1.h"
#include "ID3D10Device1.h"
#include "rpc_hwnd.h"

void    ApplyD3DDeviceCreationHook()
{
    IAT_HOOK("dwmcore.dll", "d3d10_1.dll", D3D10CreateDevice1);
}

void    RemoveD3DDeviceCreationHook()
{
    HOOK_GET_ORIGINAL_PROC(D3D10CreateDevice1);
    IAT_UNHOOK("dwmcore.dll", "d3d10_1.dll", D3D10CreateDevice1);
}

void    ApplyD3DHooks(ID3D10Device1 *pDevice)
{
    DX_METHOD_HOOK(pDevice, ID3D10Device1, Draw);
    DX_METHOD_HOOK(pDevice, ID3D10Device1, CreateInputLayout);
    DX_METHOD_HOOK(pDevice, ID3D10Device1, CreateTexture2D);
}

void    RemoveD3DHooks(ID3D10Device1 *pDevice)
{
    DX_HOOK_GET_ORIGINAL_PROC(ID3D10Device1, Draw);
    DX_HOOK_GET_ORIGINAL_PROC(ID3D10Device1, CreateInputLayout);
    DX_HOOK_GET_ORIGINAL_PROC(ID3D10Device1, CreateTexture2D);

    DX_METHOD_UNHOOK(pDevice, ID3D10Device1, Draw);
    DX_METHOD_UNHOOK(pDevice, ID3D10Device1, CreateInputLayout);
    DX_METHOD_UNHOOK(pDevice, ID3D10Device1, CreateTexture2D);
}
