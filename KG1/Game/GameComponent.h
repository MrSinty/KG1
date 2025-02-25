#pragma once

#include "../Support/DXInclude.h"
#include "TriangleComponent.h"

struct points_indexes {
    std::vector<DirectX::XMFLOAT4> points;
    std::vector<int> indexes;
};

class GameComponent
{
public:
    bool Init(HWND hWnd, Microsoft::WRL::ComPtr<ID3D11Device>& dev, 
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dContext, 
        std::vector<points_indexes> data);
    bool Draw();
    void UpdateTriangleOffset(float x, float y);
    void Shutdown();

private:

    std::vector<TriangleComponent> triangleBuff;
    std::shared_ptr<ConstantBuffer<ConstBuffVertexshader>> constBuffVS;
};

