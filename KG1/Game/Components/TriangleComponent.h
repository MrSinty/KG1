#pragma once

#include "../../Support/DXInclude.h"
#include "GameComponent.h"
#include "../ConstatntBuffer/ConstantBuffer.h"
#include <DirectXMath.h>
#include "SimpleMath.h"
#include <iostream>
#include <vector>

using namespace DirectX::SimpleMath;

class TriangleComponent : public GameComponent
{

public:
    TriangleComponent(Microsoft::WRL::ComPtr<ID3D11Device>& dev,
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dContext
    );
    ~TriangleComponent();

    bool Init() override;
    bool Draw() override;
    void Update() override;
    bool CompileShader();
    bool CreateVPBuffer();
    void SetLocationAndForm(Vector2 center, float width, float height, float depth);
    void SetMatricies(Matrix& wrld, Matrix& view, Matrix& proj);
    void SetVertexCoordinates(std::vector<Vector2>& points);
    void SetOffset(float x, float y);
    void SetColor(float r, float g, float b, float a);
    void SetScale(float scale);
    void UpdateOffset(float x, float y);
    Vector4 GetLocationAndForm();
    void ClearData();

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

    std::vector<Vector4> renderPoints = {
        Vector4(-1.0f, 1.0f, -1.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f),
        Vector4(1.0f, 1.0f, -1.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f),
        Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f),
        Vector4(-1.0f, 1.0f, 1.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f),
        Vector4(-1.0f, -1.0f, -1.0f, 1.0f), Vector4(1.0f, 0.0f, 1.0f, 1.0f),
        Vector4(1.0f, -1.0f, -1.0f, 1.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f),
        Vector4(1.0f, -1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
        Vector4(-1.0f, -1.0f, 1.0f, 1.0f), Vector4(0.0f, 0.0f, 0.0f, 1.0f),
    };

    //std::vector<int> indices = { 0,1,2, 1,0,3, 4,5,6, 5,4,7 };
    std::vector<int> indices = 
    {
        3,1,0,
        2,1,3,

        0,5,4,
        1,5,0,

        3,4,7,
        0,4,3,

        1,6,5,
        2,6,1,

        2,7,6,
        3,7,2,

        6,4,5,
        7,4,6,
    };

    Matrix World;
    Matrix View;
    Matrix Proj;
    Vector2 centerPoint = { 0.f, 0.f };
    float width = 1.0f;
    float height = 1.0f;
    float depth = 1.0f;
    Vector4 offset = { 0.0f, 0.0f, 0.0f, 0.0f };
    Vector4 color = { 0.0f, 0.0f, 0.0f, 0.0f };
    float scale = 1.0f;

    UINT strides[1] = { 32 };
    UINT offsets[1] = { 0 };
};



