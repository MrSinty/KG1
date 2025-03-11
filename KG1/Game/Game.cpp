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

    CreateDepthStencilBuffer();

#ifdef _DEBUG
    DXDebug::Get().Init(device);
#endif // _DEBUG

    platformRight = new Platform(device, context);
    platformRight->Init(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    platforms.push_back(platformRight);

    auto* platformTemp = new Platform(device, context);
    platformTemp->Init(0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f);
    platforms.push_back(platformTemp);

    platformTemp = new Platform(device, context);
    platformTemp->Init(0.0f, 0.0f, 0.0f, 0.25f, 0.25f, 0.25f);
    platforms.push_back(platformTemp);

    mWorld = Matrix::Identity;

    mView = Matrix::CreateLookAt(Vector3(2.f, 2.f, 2.f),
        Vector3::Zero, Vector3::UnitY);

    mProj = Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PIDIV2,
        float(screenWidth) / float(screenHeight), 0.1f, 100.f);

    platformRight->mesh->SetMatricies(mWorld, mView, mProj);

    return true;
}

bool Game::Init()
{
    return Game::Init(L"My3DApp", 800, 800);
}

void Game::Run()
{
    //timer.SetFixedTimeStep(true);
    //timer.SetTargetElapsedSeconds(1.f / 60.f);

    isShouldExit = false;

    //prevTime = std::chrono::steady_clock::now();
    //totalTime = 0;
    //frameCount = 0;
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
    auto delta = float(timer.GetElapsedSeconds());
    static float t = 0.0f;
    t += delta;
    if (t >= 6.26f)
        t = 0.0f;


        platforms[1]->mesh->ChangeRotZ(t);
        float orbitRadius = 1.5f;
        platforms[1]->mesh->ChangeTranslation(Vector3(0.f, 0.f, orbitRadius));
        platforms[1]->mesh->ChangeRotY(t);
    

        Matrix mPlanet = platforms[1]->mesh->GetLocalMatrix();
        mPlanet = mPlanet.Invert();
        Vector3 posPlanet = platforms[1]->mesh->GetCenterPoint();
        Vector3 worldPosPlanet = DirectX::XMVector3TransformCoord(posPlanet, mPlanet);

        Matrix wrld2 = DirectX::XMMatrixTranslationFromVector(worldPosPlanet);
        wrld2 *= Matrix::CreateRotationY(t);

        platforms[2]->mesh->ChangeRotY(t);





        platforms[2]->mesh->UpdateTransform(mPlanet);
        platforms[2]->mesh->SetMatricies(mView, mProj);

        platforms[1]->mesh->UpdateTransform();
        platforms[1]->mesh->SetMatricies(mView, mProj);



    for (auto& plfm : platforms)
    {
        plfm->Update(delta);
    }


    //for (auto ball : balls)
    //{
    //    DirectX::BoundingBox col = platformLeft->GetCollider();
    //    if (ball->CheckCollision(col))
    //    {
    //        ball->isColliding = true;

    //        if (!ball->wasColliding)
    //        {
    //            ball->ChangeDirectionAfterPlatform(col.Center, false);
    //        }

    //        ball->wasColliding = true;
    //    }

    //    platformLeft->Update(delta);


    //    col = platformRight->GetCollider();
    //    if (ball->CheckCollision(col))
    //    {
    //        ball->isColliding = true;

    //        if (!ball->wasColliding)
    //        {
    //            ball->ChangeDirectionAfterPlatform(col.Center, true);
    //        }

    //        ball->wasColliding = true;
    //    }

    //    platformRight->Update(delta);


    //    for (auto wall : walls)
    //    {
    //        col = wall->GetCollider();
    //        if (ball->CheckCollision(col))
    //        {
    //            ball->isColliding = true;

    //            if (!ball->wasColliding)
    //            {
    //                ball->ChangeDirectionAfterWall();
    //            }

    //            ball->wasColliding = true;
    //        }

    //        wall->Update(0.f);
    //    }

    //    if (ball->GetCollider().Center.x < -1.f)
    //    {
    //        NewRound(false);
    //    }

    //    if (ball->GetCollider().Center.x > 1.f)
    //    {
    //        NewRound(true);
    //    }

    //    ball->Update(delta);
    //}
}

void Game::Tick()
{
    timer.Tick([&]()
        {
            Update();
        });

    Draw();
}

void Game::Draw()
{
    float color[] = { 0.1f, 0.1f, 0.1f, 1.0f };

    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(screenWidth);
    viewport.Height = static_cast<float>(screenHeight);
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1.0f;

    context->RSSetViewports(1, &viewport);

    context->OMSetDepthStencilState(pDepthStencilState, 1);
    context->OMSetRenderTargets(1, &rtv, pDepthStencilView);

    context->ClearRenderTargetView(rtv, color);
    context->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0xFF);


    //platforms[0]->Draw();
    //platforms[1]->Draw();

    for (auto& plfm : platforms)
    {
        if (!plfm->Draw())
            return;
    }

    context->OMSetRenderTargets(0, nullptr, nullptr);

    swapChain->Present(1, 0);
}

void Game::CreateDepthStencilBuffer()
{
    D3D11_TEXTURE2D_DESC descDepth;
    descDepth.Width = screenWidth;
    descDepth.Height = screenHeight;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    if (FAILED(device->CreateTexture2D(&descDepth, NULL, &pDepthStencilBuffer)))
        return;

    D3D11_DEPTH_STENCIL_DESC dsDesc;
    // Depth test parameters
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

    // Stencil test parameters
    dsDesc.StencilEnable = true;
    dsDesc.StencilReadMask = 0xFF;
    dsDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing
    dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing
    dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create depth stencil state
    device->CreateDepthStencilState(&dsDesc, &pDepthStencilState);

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    if (FAILED(device->CreateDepthStencilView(
        pDepthStencilBuffer, // Depth stencil texture
        &descDSV, // Depth stencil desc
        &pDepthStencilView)))
        return;
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

    res = swapChain->GetBuffer(0, IID_PPV_ARGS(&backTex));
    if (FAILED(res))
        return false;

    return true;
}

void Game::AddPlatform(Platform* platformObj)
{
    platforms.push_back(platformObj);
}

void Game::AddBall(Ball* ballObj)
{
    balls.push_back(ballObj);
}

void Game::NewRound(bool isRightWin)
{
    if (isRightWin)
    {
        scoreRight++;
        std::cout << "Right WON!\n" << "The score is:\n" << "Left: " << scoreLeft << " Right: " << scoreRight << std::endl;
    }
    else
    {   
        scoreLeft++;
        std::cout << "Left WON!\n" << "The score is:\n" << "Left: " << scoreLeft << " Right: " << scoreRight << std::endl;
    }
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
    case WM_CHAR:
        InputDevice::Get().OnKeyDown(msg.wParam);

        //switch (msg.wParam)
        //{
        //case 119: // W key
        //    platformLeft->KeyDown(true);
        //    platformLeft->KeyPressed(0); // 0 - Up, 1 - Down, 2 - Right, 3 - Left
        //    break;
        //case 115: // S key
        //    platformLeft->KeyDown(true);
        //    platformLeft->KeyPressed(1);
        //    break;
        //case 100: // D key
        //    platformLeft->KeyDown(true);
        //    platformLeft->KeyPressed(2);
        //    break;
        //case 97: // A key
        //    platformLeft->KeyDown(true);
        //    platformLeft->KeyPressed(3);
        //    break;
        //}
        break;

    case WM_KEYDOWN:
        InputDevice::Get().OnKeyDown(msg.wParam);
      
        switch (msg.wParam)
        {
        case VK_LEFT:
            platformRight->KeyDown(true);
            platformRight->KeyPressed(3); // 0 - Up, 1 - Down, 2 - Right, 3 - Left
            break;
        case VK_RIGHT:
            platformRight->KeyDown(true);
            platformRight->KeyPressed(2);
            break;
        case VK_UP:
            platformRight->KeyDown(true);
            platformRight->KeyPressed(0); // 0 - Up, 1 - Down, 2 - Right, 3 - Left
            break;
        case VK_DOWN:
            platformRight->KeyDown(true);
            platformRight->KeyPressed(1);
            break;
        }
        break;
    case WM_KEYUP:
        InputDevice::Get().OnKeyUp(msg.wParam);

        switch (msg.wParam)
        {
        case VK_UP:
            platformRight->KeyDown(false);
            platformRight->KeyPressed(-1);
            break;
        case VK_DOWN:
            platformRight->KeyDown(false);
            platformRight->KeyPressed(-1);
            break;
        case VK_RIGHT:
            platformRight->KeyDown(false);
            platformRight->KeyPressed(-1);
            break;
        case VK_LEFT:
            platformRight->KeyDown(false);
            platformRight->KeyPressed(-1);
            break;
        //case 87: // W key
        //    platformLeft->KeyDown(false);
        //    platformLeft->KeyPressed(-1);
        //    break;
        //case 83: // S key
        //    platformLeft->KeyDown(false);
        //    platformLeft->KeyPressed(-1);
        //    break;
        //case 68: // D key
        //    platformLeft->KeyDown(false);
        //    platformLeft->KeyPressed(-1);
        //    //platformLeft->UpdateOffset(0.0f, 0.05f);
        //    break;
        //case 65: // A key
        //    platformLeft->KeyDown(false);
        //    platformLeft->KeyPressed(-1);
        //    //platformLeft->UpdateOffset(0.0f, -0.05f);
        //    break;
        }

        break;
    case WM_MOUSEMOVE:
        InputDevice::Get().OnMouseMove(GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam));
        break;
    case WM_QUIT:
        isShouldExit = true;
        break;
    }
}

