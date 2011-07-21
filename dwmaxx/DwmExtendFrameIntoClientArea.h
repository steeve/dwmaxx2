#pragma once

#include <Windows.h>
#include <stdio.h>
#include <d3d10_1.h>
#include <dwmapi.h>
#include "hooking.h"

DEFINE_HOOK(HRESULT, DwmExtendFrameIntoClientArea,
    HWND hWnd,
    const MARGINS *pMarInset
);
