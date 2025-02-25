#include "Game.h"

bool Game::Init(LPCWSTR appName, int width, int height)
{
    screenWidth, screenHeight = width, height;
    DisplayWin32 disp(appName, width, height);
    display = &disp;

    if (!CreateSwapChain())
        return false;

    if (!CreateRTV())
        return false;

    std::vector<points_indexes> data;
    data.push_back({
    {
        DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
        DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
        DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
    },
    { 0,1,2, 1,0,3 }
    });

    if (!gameComponent.Init(display->GetWindow(), device, context, data))
        return false;

#ifdef _DEBUG
    DXDebug::Get().Init(device);
#endif // _DEBUG


    return true;
}

bool Game::Init()
{
    return Game::Init(L"My3DApp", 800, 800);
}

void Game::Run()
{
    isShouldExit = false;

    prevTime = std::chrono::steady_clock::now();
    totalTime = 0;
    frameCount = 0;
}

void Game::Shutdown()
{
    context->ClearState();
    context->Flush();

#ifndef _DEBUG
    gameComponent.Shutdown();
#endif // !_DEBUG

    backTex->Release();
    rtv->Release();
    display->Shutdown();

#ifndef _DEBUG
    swapChain->Release();
    context->Release();
    device->Release();
#endif

#ifdef _DEBUG
    DXDebug::Get().Shutdown();
#endif // _DEBUG

}

void Game::Update()
{
    auto curTime = std::chrono::steady_clock::now();
    float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - prevTime).count() / 1000000.0f;
    prevTime = curTime;

    totalTime += deltaTime;
    frameCount++;

    if (totalTime > 1.0f) {
        float fps = frameCount / totalTime;

        totalTime -= 1.0f;

        WCHAR text[256];
        swprintf_s(text, TEXT("FPS: %f"), fps);
        SetWindowText(display->GetWindow(), text);

        frameCount = 0;
    }
}

void Game::Draw()
{
    float color[] = { totalTime, 0.1f, 0.1f, 1.0f };

    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(screenWidth);
    viewport.Height = static_cast<float>(screenHeight);
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1.0f;

    context->RSSetViewports(1, &viewport);

    context->OMSetRenderTargets(1, &rtv, nullptr);

    gameComponent.Draw();
    
    context->ClearRenderTargetView(rtv, color);

    context->DrawIndexed(6, 0, 0);

    context->OMSetRenderTargets(0, nullptr, nullptr);

    swapChain->Present(1, 0);
}

bool Game::CreateRTV()
{
    auto res = device->CreateRenderTargetView(backTex, nullptr, &rtv);
    
    if (FAILED(res))
        return false;

    return true;
}

bool Game::CreateSwapChain()
{
    // Swap Chain Descriptor
    D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

    DXGI_SWAP_CHAIN_DESC swapDesc = {};
    swapDesc.BufferCount = 2;
    swapDesc.BufferDesc.Width = screenWidth;
    swapDesc.BufferDesc.Height = screenHeight;
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.OutputWindow = display->GetWindow();
    swapDesc.Windowed = true;
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swapDesc.SampleDesc.Count = 1;
    swapDesc.SampleDesc.Quality = 0;


    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

    auto res = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevel,
        1,
        D3D11_SDK_VERSION,
        &swapDesc,
        &swapChain,
        &device,
        nullptr,
        &context
    );

    if (FAILED(res))
        return false;

    res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backTex);	// __uuidof(ID3D11Texture2D)
    if (FAILED(res))
        return false;

    return true;
}

void Game::MessageHandler()
{
    msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    switch (msg.message)
    {
    case WM_KEYDOWN:
        InputDevice::Get().OnKeyDown(msg.wParam);

        switch (msg.wParam)
        {
        case VK_LEFT:
            gameComponent.UpdateTriangleOffset(-0.1f, 0.0f);
            break;
        case VK_RIGHT:
            gameComponent.UpdateTriangleOffset(0.1f, 0.0f);
            break;
        case VK_UP:
            gameComponent.UpdateTriangleOffset(0.0f, 0.1f);
            break;
        case VK_DOWN:
            gameComponent.UpdateTriangleOffset(0.0f, -0.1f);
            break;
        }
        break;
    case WM_KEYUP:
        InputDevice::Get().OnKeyUp(msg.wParam);
        break;
    case WM_MOUSEMOVE:
        InputDevice::Get().OnMouseMove(GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam));
        break;
    case WM_QUIT:
        isShouldExit = true;
        break;
    }
}

