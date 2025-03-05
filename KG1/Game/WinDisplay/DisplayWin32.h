#pragma once
#include "../../Support/DXInclude.h"
#include <iostream>

class DisplayWin32
{
public:
    DisplayWin32(LPCWSTR appName, int width, int height);
    DisplayWin32();
    void Shutdown();

    inline HWND& GetWindow()
    {
        return hWnd;
    }

    inline HINSTANCE& GetHInstance()
    {
        return hInstance;
    }

    inline ATOM& GetWndClass()
    {
        return wndClass;
    }

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

    LPCWSTR applicationName = L"My3DApp";
    int screenWidth = 800;
    int screenHeight = 800;

    RECT windowRect = { 0, 0, 800, 800 };

    HINSTANCE hInstance = nullptr;
    HWND hWnd = nullptr;

    WNDCLASSEX wc{};
    ATOM wndClass{};
};

