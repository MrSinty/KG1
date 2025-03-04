#pragma once

#include "../Support/DXInclude.h"
#include <vector>

#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")


class GameComponent
{
public:
    virtual ~GameComponent() = default;

    virtual bool Init(Microsoft::WRL::ComPtr<ID3D11Device>& dev,
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dContext) = 0;
    virtual bool Draw() = 0;

protected:
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;




//public:
//    bool Init(HWND hWnd, Microsoft::WRL::ComPtr<ID3D11Device>& dev, 
//        Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dContext, 
//        std::vector<points_indexes> data);
//    bool Draw();
//    void UpdateTriangleOffset(float x, float y);
//    void Shutdown();
//
//private:
//
//    std::vector<TriangleComponent> triangleBuff;
//    std::shared_ptr<ConstantBuffer<ConstBuffVertexshader>> constBuffVS;
};

