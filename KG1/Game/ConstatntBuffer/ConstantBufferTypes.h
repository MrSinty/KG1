#pragma once
#include <DirectXMath.h>

struct ConstBuffVertexshader
{
    DirectX::XMMATRIX mWorld;
    DirectX::XMMATRIX mView;
    DirectX::XMMATRIX mProjection;
    DirectX::XMFLOAT4 offset;
    DirectX::XMFLOAT4 color;
    float scale;
    float padding[3];
};

struct CB_PS_pixelshader
{
    float alpha = 1.0f;
};