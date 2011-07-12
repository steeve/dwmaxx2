#include <map>
#include "ID3D10Device1.h"
#include "globals.h"
#include "constants.h"
#include "types.h"
#include "texture_technique.h"
#include "vertices_technique.h"

METHOD_HOOK(void, ID3D10Device1, Draw, UINT VertexCount,
                                       UINT StartVertexLocation)
{
    __ID3D10Device1_Draw(This, VertexCount, StartVertexLocation);

    ID3D10InputLayout *inputLayout = NULL;
    This->IAGetInputLayout(&inputLayout);
    
    if (inputLayout != NULL && g_inputLayouts[inputLayout] == 4) // are we drawing a window ?
    {
        // Retrieve current texture
        ID3D10ShaderResourceView *shaderResource;
        This->PSGetShaderResources(0, 1, &shaderResource);
        ID3D10Resource *pResource = NULL;
        shaderResource->GetResource(&pResource);

        ID3D10Texture2D *currentTexture = NULL;
        pResource->QueryInterface(__uuidof(ID3D10Texture2D), (void **)&currentTexture);

        D3D10_TEXTURE2D_DESC currentTextureDesc;
        currentTexture->GetDesc(&currentTextureDesc);

        HANDLE textureSharedHandle;
        UINT dataSize;
        if ((currentTextureDesc.BindFlags & D3D10_BIND_SHADER_RESOURCE) != 0
            && currentTextureDesc.CPUAccessFlags == 0
            && currentTextureDesc.Usage == D3D10_USAGE_DEFAULT
            && currentTextureDesc.Format == DXGI_FORMAT_B8G8R8A8_UNORM
            && currentTextureDesc.MiscFlags == D3D10_RESOURCE_MISC_SHARED) // are we trying to create a window? most likely?
        {
            if (currentTexture->GetPrivateData(WatermarkGuid, &dataSize, &textureSharedHandle) == DXGI_ERROR_NOT_FOUND)
            {
                // Read the watermark
                HWND hWnd = GetWindowFromTexture(This, currentTexture, &currentTextureDesc);
                if (hWnd != NULL && IsWindowVisible(hWnd) == TRUE) // no need to use hidden windows
                {
                    // Get DXGI resource
                    IDXGIResource *dxgiResource = NULL;
                    pResource->QueryInterface(__uuidof(IDXGIResource), (void **)&dxgiResource);

                    // Get shared handle on resource
                    HANDLE dxgiSharedHandle = NULL;
                    HRESULT hr = dxgiResource->GetSharedHandle(&textureSharedHandle);
                    dxgiResource->Release();
                    if (SUCCEEDED(hr))
                    {
                        g_sharedHandles[hWnd] = textureSharedHandle;
                        currentTexture->SetPrivateData(WatermarkGuid, (UINT)sizeof(textureSharedHandle), &textureSharedHandle);
#ifdef _DEBUG
                        char title[1024];
                        GetWindowText(hWnd, title, sizeof(title));
                        printf("ADDED: %s (hWnd=0x%08I64x, texHandle=0x%08I64x)\n", title, hWnd, textureSharedHandle);
#endif
                    }
                }
            }
        }
        currentTexture->Release();
        pResource->Release();
        shaderResource->Release();
    }
    inputLayout->Release();
}


        //D3D10_TEXTURE2D_DESC desc;
        //currentTexture->GetDesc(&desc);

        //
        //if (desc.Width != 597 && desc.Height != 409)
        //{
        //    //printf("%ux%u\n", desc.Width, desc.Height);
        //    if (desc.Width == 800 && desc.Height == 479)
        //    {
        //        printf("NOTEPAD\n");
        //        //SaveToFile(This, currentTexture, "C:\\dev\tex.dds");
        //        DWMWindowPixel pixel;
        //        ReadPixel(This, currentTexture, 0, 0, &pixel);
        //        printf("%x\n", pixel.color);
        //    }
        //}



//        if (abs(800 - cx) < 50)
//        {
//
//
//            //D3D10_TEXTURE2D_DESC desc;
//            //pTex->GetDesc(&desc);
//
//
//            pTex->Release();
//
//            //// Get DXGI resource
//            //IDXGIResource *dxgiResource = NULL;
//            //pResource->QueryInterface(__uuidof(IDXGIResource), (void **)&dxgiResource);
//
//            //// Get shared handle on resource
//            //HANDLE dxgiSharedHandle = NULL;
//            //HRESULT hr = dxgiResource->GetSharedHandle(&dxgiSharedHandle);
//            //g_sharedHandles[hWnd] = dxgiSharedHandle;
//
//            //// Release the resource
//            //dxgiResource->Release();
//            pResource->Release();
//            shaderResource->Release();
//
//            //printf("Drawn %s - %I64x\n", title, dxgiSharedHandle);
//        }
//
//        return;
//    }
//    inputLayout->Release();
//}
        //printf("%d => X=%.2f Y=%.2f Z=%.2f\n", g_InputLayouts[inputLayout], vertex.Position.x, vertex.Position.y, vertex.Position.z);
        //    printf("X=%.2f Y=%.2f Z=%.2f - T1: %u %u %u %u - T2: %0.2f %0.2f - T3: %0.2f %0.2f\n", vertex.Position.x,
        //                                                                                            vertex.Position.y,
        //                                                                                            vertex.Position.z,
        //                                                                                            vertex.Tex1Coord.r,
        //                                                               k                             vertex.Tex1Coord.g,
        //                                                                                            vertex.Tex1Coord.b,
        //                                                                                            vertex.Tex1Coord.a,
        //                                                                                            vertex.Tex2Coord.x,
        //                                                                                            vertex.Tex2Coord.y,
        //                                                                                            vertex.Tex3Coord.x,
        //                                                                                            vertex.Tex3Coord.y);
        //    vertex = vertices[1];
        //    printf("X=%.2f Y=%.2f Z=%.2f - T1: %u %u %u %u - T2: %0.2f %0.2f - T3: %0.2f %0.2f\n\n", vertex.Position.x,
        //                                                                                            vertex.Position.y,
        //                                                                                            vertex.Position.z,
        //                                                                                            vertex.Tex1Coord.r,
        //                                                                                            vertex.Tex1Coord.g,
        //                                                                                            vertex.Tex1Coord.b,
        //                                                                                            vertex.Tex1Coord.a,
        //                                                                                            vertex.Tex2Coord.x,
        //                                                                                            vertex.Tex2Coord.y,
        //                                                                                            vertex.Tex3Coord.x,
        //                                                                                            vertex.Tex3Coord.y);
        //}



        //// Get shader resource
    //    ID3D10Resource *pResource = NULL;
    //    shaderResources[0]->GetResource(&pResource);

    //    ID3D10Texture2D *pTex = NULL;
    //    pResource->QueryInterface(__uuidof(ID3D10Texture2D), (void **)&pTex);

    //    D3D10_TEXTURE2D_DESC desc;
    //    pTex->GetDesc(&desc);
    //    pTex->Release();

    //    if (desc.Width == 1012 && desc.Height == 733)
    //    {
    //        printf("CHROME !!!\n");

    //        //printf("flags: %u - %u\n", desc.MiscFlags & D3D10_RESOURCE_MISC_SHARED, desc.MiscFlags & D3D10_RESOURCE_MISC_SHARED_KEYEDMUTEX);

    //        // Get DXGI resource
    //        IDXGIResource *dxgiResource = NULL;
    //        pResource->QueryInterface(__uuidof(IDXGIResource), (void **)&dxgiResource);

    //        // Get shared handle on resource
    //        HANDLE dxgiSharedHandle = NULL;
    //        HRESULT hr = dxgiResource->GetSharedHandle(&dxgiSharedHandle);

    //        // Release the resource
    //        dxgiResource->Release();
    //    }
    //    pResource->Release();
//    }
//}

        //    // try to reopen the handle:
        //    //ID3D10Texture2D *pTexture = NULL;

        //    //HRESULT hrs = This->OpenSharedResource((HANDLE)((DWORD)dxgiSharedHandle & 0xFFFFFFFF), __uuidof(ID3D10Texture2D), (void**)&pTexture);

        //    //printf("HR: %x -> 0x%I64X\n", hrs, pTexture);

        //}

        //printf("%.2f - %.2f\n", vertices[1].Position.x - vertices[0].Position.x, vertices[2].Position.y - vertices[0].Position.y);
        //printf("%.2f - %.2f\n", vertices[3].Position.x, vertices[3].Position.y);

        //DWMVertex vertex = vertices[1];

        //printf("X=%.2f Y=%.2f - T1: %u %u %u %u - T2: %0.2f %0.2f - T3: %0.2f %0.2f\n", vertex.Position.x,
        //                                                                                    vertex.Position.y,
        //                                                                                    vertex.Tex1Coord.r,
        //                                                                                    vertex.Tex1Coord.g,
        //                                                                                    vertex.Tex1Coord.b,
        //                                                                                    vertex.Tex1Coord.a,
        //                                                                                    vertex.Tex2Coord.x,
        //                                                                                    vertex.Tex2Coord.y,
        //                                                                                    vertex.Tex3Coord.x,
        //                                                                                    vertex.Tex3Coord.y);

        // Based on vertex position, determine original hwnd
        // maybe encode hwnd in vertex position ?

        // Get current shader resource
        //ID3D10ShaderResourceView *shaderResources[1] = { NULL };
        //This->PSGetShaderResources(0, 1, shaderResources);

        //// Get shader resource
        //ID3D10Resource *pResource = NULL;
        //shaderResources[0]->GetResource(&pResource);

        //// Get DXGI resource
        //IDXGIResource *dxgiResource = NULL;
        //pResource->QueryInterface(__uuidof(IDXGIResource), (void **)&dxgiResource);

        //// Get shared handle on resource
        //HANDLE dxgiSharedHandle = NULL;
        //dxgiResource->GetSharedHandle(&dxgiSharedHandle);

        //// Release the resource
        //dxgiResource->Release();
//    }
//}
