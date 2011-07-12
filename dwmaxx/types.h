#pragma once

#include <Windows.h>
#include <d3d10_1.h>
#include <D3DX10.h>

typedef BOOL    (*ISTOPLEVELWINDOW_PROC)(HWND hWnd);

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
