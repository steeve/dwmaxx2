#include "texture_technique.h"
#include "globals.h"
#include "hooking.h"
#include "ID3D10Device1.h"
#include "..\\dwmaxx_watermarking\\constants.h"

HOOK_GET_ORIGINAL_PROC(ID3D10Device1, CreateTexture2D);

void ReadPixel(ID3D10Device1 *device, ID3D10Texture2D *pTexture, int x, int y, DWMWindowPixel *pixel)
{
    if (g_pixelTexture == NULL)
    {
        D3D10_TEXTURE2D_DESC desc;
        pTexture->GetDesc(&desc);
        desc.Width = 1;
        desc.Height = 1;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D10_USAGE_STAGING;
        desc.BindFlags = 0;
        desc.CPUAccessFlags = D3D10_CPU_ACCESS_READ;
        desc.MiscFlags = 0;
        // We don't want to call the hooked version!
        __ID3D10Device1_CreateTexture2D(device, &desc, NULL, &g_pixelTexture);
    }

	ID3D10Resource *srcRes;
    ID3D10Resource *destRes;
	pTexture->QueryInterface(__uuidof(ID3D10Resource), (void **)&srcRes);
	g_pixelTexture->QueryInterface(__uuidof(ID3D10Resource), (void **)&destRes);

    D3D10_BOX box;
    box.left = x;
    box.right = box.left + 1;
    box.top = y;
    box.bottom = box.top + 1;
    box.front = 0;
    box.back = 1;
    device->CopySubresourceRegion(destRes, 0, 0, 0, 0, srcRes, 0, &box);

    srcRes->Release();
    destRes->Release();

    D3D10_MAPPED_TEXTURE2D mapping;
    g_pixelTexture->Map(0, D3D10_MAP_READ, 0, &mapping);
    *pixel = *(DWMWindowPixel *)mapping.pData;
    g_pixelTexture->Unmap(0);
}

HWND GetWindowFromTexture(ID3D10Device1 *device, ID3D10Texture2D *pTex, D3D10_TEXTURE2D_DESC *pTexDesc)
{
    DWMWindowPixel pixel;
    ReadPixel(device, pTex, pTexDesc->Width / 2, pTexDesc->Height / 2, &pixel);
    HWND foundHwnd = (HWND)pixel.color;

    if (foundHwnd != NULL
        && foundHwnd != (HWND)0x00FFFFFF
        && foundHwnd != (HWND)0xFFFFFFFF
        && IsWindow(foundHwnd) == TRUE)
    {
        return (foundHwnd);
    }
    return (NULL);
}
