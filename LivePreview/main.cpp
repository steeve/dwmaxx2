#include <Windows.h>
#include <stdio.h>
#include <dwmapi.h>
#pragma comment (lib, "dwmapi.lib")

HWND g_hWnd = NULL;

void MakeThumbnailFor(HWND hWnd)
{
    if (IsWindowVisible(hWnd))
    {
        WINDOWINFO info;
        GetWindowInfo(hWnd, &info);
        printf("TRYING\n");
        HRESULT hr = S_OK;

        // Register the thumbnail
        HTHUMBNAIL hThumb = NULL;
        hr = DwmRegisterThumbnail(g_hWnd, hWnd, &hThumb);
        if (SUCCEEDED(hr))
        {
            // Specify the destination rectangle size
            RECT dest;

            // try our encode trick!
            //dest.left = (LONG)((DWORD)hWnd & 0x000000FF);
            //dest.top = (LONG)((DWORD)hWnd >> 8 & 0x000000FF);
            dest.left = 50;
            dest.top = 50;
            dest.bottom = dest.top + (info.rcWindow.bottom - info.rcWindow.top);
            dest.right = dest.left + (info.rcWindow.right - info.rcWindow.left);

            // Set the thumbnail properties for use
            DWM_THUMBNAIL_PROPERTIES dskThumbProps;
            dskThumbProps.dwFlags = DWM_TNP_SOURCECLIENTAREAONLY | DWM_TNP_VISIBLE | DWM_TNP_RECTDESTINATION | DWM_TNP_OPACITY;
            dskThumbProps.fSourceClientAreaOnly = FALSE; 
            dskThumbProps.fVisible = TRUE;
            dskThumbProps.rcDestination = dest;
            dskThumbProps.opacity = 255;
            //dskThumbProps.opacity = (BYTE)((DWORD)hWnd >> 16 & 0x000000FF);

            // Display the thumbnail
            hr = DwmUpdateThumbnailProperties(hThumb, &dskThumbProps);
            hr = S_OK;
        }
    }
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch (msg)
    {
    case WM_APP:
        {
            //HWND hWnd = (HWND)0x0022089E; //FindWindow("Chrome_WidgetWin_0", NULL);
            //MakeThumbnailFor(hWnd);

            HWND win = NULL;
            do
            {
                win = FindWindowEx(NULL, win, "Chrome_WidgetWin_0", NULL);
            } while (GetWindowTextLength(win) == 0);

            MakeThumbnailFor(win);

            break;
        }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int CALLBACK WinMain(
  __in  HINSTANCE hInstance,
  __in  HINSTANCE hPrevInstance,
  __in  LPSTR lpCmdLine,
  __in  int nCmdShow
)
{
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW ;
    wcex.lpfnWndProc = &WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetModuleHandle(NULL);
    wcex.hIcon = NULL;
    wcex.hCursor = NULL;
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = NULL; 
    wcex.lpszClassName = "DwmaxxHwndRegistry";
    wcex.hIconSm = NULL;//LoadIcon(NULL, IDI_APPLICATION);
    //wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )107  );
    if( !RegisterClassEx( &wcex ) ){
        //fout << clock() << " "  << "Windows Class Creation failed" << endl;
    }
    //	return E_FAIL;

    g_hWnd = CreateWindowEx(/*WS_EX_LAYERED | WS_EX_TRANSPARENT | */WS_EX_TOPMOST, "DwmaxxHwndRegistry", "Some Title", WS_VISIBLE | WS_POPUP, 0, 0, 51, 51, NULL, NULL, NULL, NULL);

    //LONG winLong = GetWindowLong(g_hWnd, GWL_EXSTYLE);
    //SetWindowLongPtr(g_hWnd, GWL_EXSTYLE, winLong | WS_EX_LAYERED);
    //SetLayeredWindowAttributes(g_hWnd, 0, 1, LWA_ALPHA);

    ShowWindow(g_hWnd, SW_SHOW);
    PostMessage(g_hWnd, WM_APP, NULL, NULL);

    MSG  msg;
    while( GetMessage(&msg, NULL, 0, 0))
    {
        DispatchMessage(&msg);
    }
    return (int) msg.wParam;
}
