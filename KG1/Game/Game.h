#pragma once

#include "GameComponent.h"
#include "../Support/DXInclude.h"
#include "DisplayWin32.h"
#include "InputDevice.h"
#include "../Debug/DXDebug.h"

#include <windowsx.h>
#include <iostream>
#include <chrono>

class Game
{
public:
    bool Init(LPCWSTR appName, int width, int height);
    bool Init();
    void Run();
    void Shutdown();
    void Update();
    void UpdateInternal();
    void Draw();
    void MessageHandler();

    inline Microsoft::WRL::ComPtr<ID3D11Device>& GetDevice()
    {
        return device;
    }

    inline Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetDeviceContext()
    {
        return context;
    }

    inline Microsoft::WRL::ComPtr<IDXGISwapChain>& GetSwapChain()
    {
        return swapChain;
    }

    inline DisplayWin32& GetDisplay()
    {
        return *display;
    }

    inline bool GetShouldClose()
    {
        return isShouldExit;
    }

private:

    bool isShouldExit = false;

    LPCWSTR applicationName = L"My3DApp";
    int screenWidth = 800;
    int screenHeight = 800;

    DisplayWin32* display;

    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr <ID3D11DeviceContext> context;
    Microsoft::WRL::ComPtr <IDXGISwapChain> swapChain;

    ID3D11Texture2D* backTex = nullptr;
    ID3D11RenderTargetView* rtv = nullptr;

    GameComponent gameComponent = GameComponent();

    MSG msg = {};
    std::chrono::time_point<std::chrono::steady_clock> prevTime{};
    float totalTime = 0;
    UINT frameCount = 0;


private:
    bool CreateRTV();
    bool CreateSwapChain();
    void EndFrame();
    void Exit();
    void PrepareFrame();
    void PrepareResources();
    void RestoreTargets();


public:
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    inline static Game& Get()
    {
        static Game instance;
        return instance;
    }

private:
    Game() = default;
};

