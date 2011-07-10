#include <stdio.h>
#include "vertices_technique.h"

HWND DecodeAXYToHWND(int alpha, int x, int y)
{
    WINDOWINFO info;
    GetWindowInfo(FindWindow("DwmaxxHwndRegistry", NULL), &info);

    DWORD hWnd = NULL;
    hWnd |= (x - info.rcClient.left) & 0xFF;
    hWnd |= ((y - info.rcClient.top) & 0xFF) << 8;
    hWnd |= (0xFF - (alpha & 0xFF)) << 16;

    return ((HWND)hWnd);
}

HWND GetWindowFromVertices(DWMVertex *vertices)
{
    // DWM create the triangles like this:
    //
    //  0     1
    //   +---+
    //   |\  |
    //   | \ |
    //   |  \|
    //   +---+
    //  3     2

    HWND hWnd = DecodeAXYToHWND((int)vertices[0].Tex1Coord.r, (int)vertices[0].Position.x, (int)vertices[0].Position.y);

    if (hWnd != NULL && hWnd != (HWND)0xFFFFFF && hWnd != (HWND)0xFFFFFFFF && IsWindow(hWnd) == TRUE)
        return (hWnd);
    return (NULL);
}

ID3D10Buffer    *tmpBuffer = NULL;

void ReadVertices(ID3D10Device1 *device, DWMVertex *vertices, ID3D10Buffer *vertexBuffer, int VertexCount, int StartVertexLocation)
{
    HRESULT             res = NULL;
    //ID3D10Buffer        *tmpBuffer = NULL;
    if (tmpBuffer == NULL)
    {
	    D3D10_BUFFER_DESC   desc;
        vertexBuffer->GetDesc(&desc);
	    desc.Usage = D3D10_USAGE_STAGING;
	    desc.CPUAccessFlags |= D3D10_CPU_ACCESS_READ;
	    desc.BindFlags = 0;
        desc.MiscFlags = 0;
        res = device->CreateBuffer(&desc, NULL, &tmpBuffer);
    }

	ID3D10Resource *SrcRes;
    ID3D10Resource *DestRes;
	vertexBuffer->QueryInterface(__uuidof(ID3D10Resource), (void **)&SrcRes);
	tmpBuffer->QueryInterface(__uuidof(ID3D10Resource), (void **)&DestRes);

    D3D10_BOX box;
    box.left = StartVertexLocation * sizeof(DWMVertex);
    box.right = (StartVertexLocation + VertexCount) * sizeof(DWMVertex);
    box.top = box.front = 0;
    box.bottom = box.back = 1;
    device->CopySubresourceRegion(DestRes, 0, 0, 0, 0, SrcRes, 0, &box);

	SrcRes->Release();
	DestRes->Release();

    void *pData;
    tmpBuffer->Map(D3D10_MAP_READ, 0, (void**)&pData);
    memcpy(vertices, pData, sizeof(DWMVertex) * VertexCount);
    tmpBuffer->Unmap();
    //tmpBuffer->Release();
}
