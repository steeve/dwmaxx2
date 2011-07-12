#pragma once
#define DWM_MODULE      ("dwm.exe")

#ifdef _WIN64
#define DWMAXX_WATERMARKER_CLASS    ("DwmaxxWatermarker64")
#else
#define DWMAXX_WATERMARKER_CLASS    ("DwmaxxWatermarker32")
#endif

// {BDC90ECB-C437-41CD-B258-5607E14F7FDE}
static const GUID WatermarkGuid = 
{ 0xbdc90ecb, 0xc437, 0x41cd, { 0xb2, 0x58, 0x56, 0x7, 0xe1, 0x4f, 0x7f, 0xde } };
