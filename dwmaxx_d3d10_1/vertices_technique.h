#pragma once
#include <Windows.h>
#include <d3d10_1.h>
#include "types.h"

HWND DecodeAXYToHWND(int alpha, int x, int y);
HWND GetWindowFromVertices(DWMVertex *vertices);
void ReadVertices(ID3D10Device1 *device, DWMVertex *vertices, ID3D10Buffer *vertexBuffer, int VertexCount, int StartVertexLocation);
