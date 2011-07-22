#pragma once
#include <Windows.h>
#include <d3d10_1.h>
#include <D3DX10.h>
#include <dwmapi.h>

typedef struct _LOCAL_HOOK_INFO_* PLOCAL_HOOK_INFO;

typedef struct _HOOK_TRACE_INFO_
{
    PLOCAL_HOOK_INFO        Link;
}HOOK_TRACE_INFO, *TRACED_HOOK_HANDLE;

typedef BOOL        (WINAPI *ISTOPLEVELWINDOW_PROC)(HWND hWnd);
typedef NTSTATUS    (WINAPI *LHINSTALLHOOK_PROC)(void* InEntryPoint, void* InHookProc, void* InCallback, TRACED_HOOK_HANDLE OutHandle);
typedef NTSTATUS    (WINAPI *LHUNINSTALLALLHOOKS_PROC)();
typedef NTSTATUS    (WINAPI *LHUNINSTALLHOOK_PROC)(TRACED_HOOK_HANDLE InHandle);
typedef NTSTATUS    (WINAPI *LHSETEXCLUSIVEACL_PROC)(ULONG* InProcessIdList, ULONG InProcessCount, TRACED_HOOK_HANDLE InHandle);

typedef union
{
    struct
    {
        DWORD b : 8;
        DWORD g : 8;
        DWORD r : 8;
        DWORD a : 8;
    };
    DWORD   color;
} DWMVertexColor;

typedef struct
{
    D3DXVECTOR3     Position;
    DWMVertexColor  Tex1Coord;
    D3DXVECTOR2     Tex2Coord;
    D3DXVECTOR2     Tex3Coord;
} DWMVertex;

typedef union
{
    struct
    {
        DWORD b : 8;
        DWORD g : 8;
        DWORD r : 8;
        DWORD a : 8;
    };
    DWORD   color;
} DWMWindowPixel;

typedef struct
{
    HWND    hWnd;
    HANDLE  sharedTextureHandle;
    MARGINS dwmMargins;
} DwmaxxWindowEntry;
