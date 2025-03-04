#pragma once

#include "../Support/DXInclude.h"
#include "GameComponent.h"
#include "ConstantBuffer.h"
#include <DirectXMath.h>
#include <d3d11.h>
//#include "SimpleMath.h"
#include <d3dcompiler.h>
#include <iostream>
#include <vector>

class TriangleComponent : GameComponent
{

public:
    TriangleComponent(Microsoft::WRL::ComPtr<ID3D11Device>& dev,
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dContext
    );

    bool Init(Microsoft::WRL::ComPtr<ID3D11Device>& dev,
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dContext) override;

    bool Draw() override;
    bool CompileShader();
    bool CreateVPBuffer();
    void SetPosition(float x, float y);
    void SetColor(float r, float g, float b, float a);
    void UpdateOffset(float x, float y);

private:
    ConstantBuffer<ConstBuffVertexshader> constBuff;
    ID3D11InputLayout* layout = nullptr;
    ID3D11PixelShader* pixelShader = nullptr;
    ID3DBlob* pixelShaderByteCode = nullptr;
    ID3D11VertexShader* vertexShader = nullptr;
    ID3DBlob* vertexShaderByteCode = nullptr;

    ID3D11RasterizerState* rastState = nullptr;

    ID3D11Buffer* vertBuff = nullptr;

    ID3D11Buffer* indBuff = nullptr;

    std::vector<DirectX::XMFLOAT4> renderPoints = {
        DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
        DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
        DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
    };
    std::vector<int> indexes = { 0,1,2, 1,0,3 };

    DirectX::XMFLOAT4 offset = { 0.0f, 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT4 color = { 0.0f, 1.0f, 1.0f, 1.0f };

    float positionX = 0.0f;
    float positionY = 0.0f;

    UINT strides[1] = { 32 };
    UINT offsets[1] = { 0 };

//public:
//
//    bool Init (HWND hWnd, 
//        Microsoft::WRL::ComPtr<ID3D11Device>& dev, 
//        Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dContext, 
//        std::shared_ptr < ConstantBuffer<ConstBuffVertexshader>> constBuffVS,
//        std::vector<DirectX::XMFLOAT4> points, 
//        std::vector<int> indexes);
//    bool CompileShader();
//    bool CreateVPBuffer();
//    bool Draw();
//    void ClearData();
//    void UpdateOffset(float x, float y);
//
//private:
//    HWND window{};
//    Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
//    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = nullptr;
//
//    ID3D11InputLayout* layout = nullptr;
//    ID3D11PixelShader* pixelShader = nullptr;
//    ID3DBlob* pixelShaderByteCode = nullptr;
//    ID3D11VertexShader* vertexShader = nullptr;
//    ID3DBlob* vertexShaderByteCode = nullptr;
//
//    ID3D11RasterizerState* rastState = nullptr;
//
//    ID3D11Buffer* vertBuff = nullptr;
//
//    ID3D11Buffer* indBuff = nullptr;
//
//    std::vector<DirectX::XMFLOAT4> renderPoints;
//    std::vector<int> indexes;
//
//    std::shared_ptr <ConstantBuffer<ConstBuffVertexshader>> constBuffVertexshader;
//
//    DirectX::XMFLOAT4 offset = { 0.0f, 0.0f, 0.0f, 0.0f };
//
//    UINT strides[1] = { 32 };
//    UINT offsets[1] = { 0 };
};



