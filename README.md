DWMaxx2
========

This library purpose is to extract a valid DirectX 10 texture from any window on Windows 7.
Please note that I don't support it anymore, but I released the code for people to use.

See it in action: http://youtu.be/Fpvh16VjrRE http://youtu.be/TQWErKtfpq4


How it works
============

The library injects itself system wide, but behaves differently depending on
which process it's in.

If it's not in the DWM process, it will hook all draws and draw a special pixel
in the center of the window containing the HWND encoded in RGB.

If the process is DWM, it will hook itself on DX10's drawing calls and look for
this special pixel.

It will then maintain a `HWND <--> TextureHandle` mapping, allowing the DWMaxx2
API to query it for the texture shared handle for a particular HWND.


How to use
==========

Look at the code in `Tutorial07`. This is the modified Tutorial 7 from the
DX10 SDK, modified to get the texture of a Chrome window (class `Chrome_WidgetWin_0`),
so it should be pretty simple for everyone.


Methods provided:
=================
```
HRESULT  DwmaxxLoad();
HRESULT  DwmaxxUnload();
BOOL     DwmaxxIsLoaded();

HANDLE   DwmaxxGetWindowSharedHandle(HWND hWnd);
void     DwmaxxGetWindowSharedHandleAsync(HWND hWnd, HWND callbackHwnd);
void     DwmaxxRemoveWindow(HWND hWnd);
```