#pragma once
#include <DirectXMath.h>

struct ConstBuffVertexshader
{
    DirectX::XMFLOAT4 offset;
    DirectX::XMFLOAT4 color;
};

struct CB_PS_pixelshader
{
    float alpha = 1.0f;
};