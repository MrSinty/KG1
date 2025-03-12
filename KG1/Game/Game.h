#pragma once

#include "Components/TriangleComponent.h"
#include "Planet.h"
#include "Platform.h"
#include "Ball.h"
#include "../Support/DXInclude.h"
#include "WinDisplay/DisplayWin32.h"
#include "Input/InputDevice.h"
#include "../DXDebug/DXDebug.h"
#include "StepTimer.h"

#include <GeometricPrimitive.h>
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
    void Tick();
    void Draw();
    void MessageHandler();
    void AddPlatform(Platform* platformObj);
    void AddBall(Ball* ballObj);
    void NewRound(bool isRightWin);

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

    ID3D11Texture2D* pDepthStencilBuffer;
    ID3D11DepthStencilState* pDepthStencilState;
    ID3D11DepthStencilView* pDepthStencilView;

    DirectX::SimpleMath::Matrix mWorld;
    DirectX::SimpleMath::Matrix mView;
    DirectX::SimpleMath::Matrix mProj;

    std::vector<Planet*> planets;
    std::vector<Platform*> platforms;
    Platform* platformLeft = nullptr;
    Platform* platformRight = nullptr;

    int scoreLeft = 0;
    int scoreRight = 0;

    MSG msg = {};
    DX::StepTimer timer;

private:
    bool CreateRTV();
    bool CreateSwapChain();
    void CreateDepthStencilBuffer();
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

