#include "DisplayWin32.h"

DisplayWin32::DisplayWin32(LPCWSTR appName, int width, int height) : applicationName(appName), screenWidth(width), screenHeight(height)
{

    hInstance = GetModuleHandle(nullptr);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = applicationName;
    wc.cbSize = sizeof(WNDCLASSEX);

    wndClass = RegisterClassEx(&wc);

    windowRect = { 0, 0, static_cast<LONG>(screenWidth), static_cast<LONG>(screenHeight) };

    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;

    auto posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
    auto posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

    hWnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        applicationName,
        applicationName,
        dwStyle,
        posX, 
        posY,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr, 
        nullptr, 
        hInstance, 
        nullptr
    );

    ShowWindow(hWnd, SW_SHOW);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);
    ShowCursor(true);
}

DisplayWin32::DisplayWin32()
{
    DisplayWin32(L"My3DApp", 800, 800);
}

void DisplayWin32::Shutdown()
{
    DestroyWindow(hWnd);
    UnregisterClassW((LPCWSTR)wndClass, GetModuleHandle(nullptr));
}

LRESULT CALLBACK DisplayWin32::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
    switch (umessage)
    {
    case WM_KEYDOWN:
    {
        std::cout << "Key: " << static_cast<unsigned int>(wparam) << std::endl;
        if (static_cast<unsigned int>(wparam) == 27) PostQuitMessage(0);
        return 0;
    }
    default:
    {
        return DefWindowProc(hwnd, umessage, wparam, lparam);
    }
    }
}
