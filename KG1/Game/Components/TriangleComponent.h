#pragma once

#include "../../Support/DXInclude.h"
#include "GameComponent.h"
#include "../ConstatntBuffer/ConstantBuffer.h"
//#include "../Transform/Transform.h"
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

    bool Init(Transform* transform) override;
    bool Draw() override;
    void Update() override;
    bool CompileShader();
    bool CreateVPBuffer();
    void SetLocationAndForm(Vector3 center, float width, float height, float depth);

    //void ChangeScale(Vector3 scale);
    //void ChangeRotX(float radians);
    //void ChangeRotY(float radians);
    //void ChangeRotZ(float radians);
    //void ChangeTranslation(Vector3 distance);
    //void RotateAroundPoint(Vector3 pivot, float angle);
    //void UpdateTransform();
    //void UpdateTransform(const Matrix& parentWorld);
    //void UpdateWorldMatrix();
    //void UpdateWorldMatrix(const Matrix& parentMatrix);
    //void SetMatricies(Matrix& wrld, Matrix& view, Matrix& proj);
    void SetMatricies(Matrix& view, Matrix& proj);

    void SetVertexCoordinates(std::vector<Vector3>& points);
    void SetOffset(float x, float y);
    void SetColor(float r, float g, float b, float a);
    void UpdateOffset(float x, float y);
    Vector4 GetLocationAndForm();
    Vector3 GetStartPoint();
    void ClearData();

    //DirectX::SimpleMath::Vector3 Translation() const { return translation; }
    //void Translation(DirectX::SimpleMath::Vector3 val) { translation = val; }
    //DirectX::SimpleMath::Vector3 Rotation() const { return rotation; }
    //void Rotation(DirectX::SimpleMath::Vector3 val) { rotation = val; }
    //DirectX::SimpleMath::Vector3 Dist() const { return dist; }
    //void Dist(DirectX::SimpleMath::Vector3 val) { dist = val; }
    //float Period() const { return period; }
    //void Period(float val) { period = val; }
    //DirectX::SimpleMath::Vector3 Position() const { return position; }
    //void Position(DirectX::SimpleMath::Vector3 val) { position = val; }
    //DirectX::SimpleMath::Vector3 Scale() const { return scale; }
    //void Scale(DirectX::SimpleMath::Vector3 val) { scale = val; }
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
        Vector4(0.1f, 0.1f, 0.1f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f),
        Vector4(-0.1f, -0.1f, 0.1f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f),
        Vector4(0.1f, -0.1f, 0.1f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f),
        Vector4(-0.1f, 0.1f, 0.1f, 1.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f),
        Vector4(0.1f, 0.1f, -0.1f, 1.0f), Vector4(1.0f, 0.0f, 1.0f, 1.0f),
        Vector4(-0.1f, -0.1f, -0.1f, 1.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f),
        Vector4(0.1f, -0.1f, -0.1f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
        Vector4(-0.1f, 0.1f, -0.1f, 1.0f), Vector4(0.2f, 0.2f, 0.2f, 1.0f)


        //Vector4(-1.0f, 1.0f, -1.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f),
        //Vector4(1.0f, 1.0f, -1.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f),
        //Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f),
        //Vector4(-1.0f, 1.0f, 1.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f),
        //Vector4(-1.0f, -1.0f, -1.0f, 1.0f), Vector4(1.0f, 0.0f, 1.0f, 1.0f),
        //Vector4(1.0f, -1.0f, -1.0f, 1.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f),
        //Vector4(1.0f, -1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
        //Vector4(-1.0f, -1.0f, 1.0f, 1.0f), Vector4(0.0f, 0.0f, 0.0f, 1.0f),
    };

    std::vector<int> indices = 
    {
        0,1,2,
        1,0,3,

        0,4,7,
        3,0,7,

        4,2,6,
        2,4,0,

        3,5,1,
        5,3,7,

        5,2,1,
        2,5,6,

        7,6,5,
        6,7,4



        //3,1,0,
        //2,1,3,

        //0,5,4,
        //1,5,0,

        //3,4,7,
        //0,4,3,

        //1,6,5,
        //2,6,1,

        //2,7,6,
        //3,7,2,

        //6,4,5,
        //7,4,6,
    };

    Matrix World;
    Matrix View;
    Matrix Proj;
    Transform* transform;
    //Vector3 translation;
    //Vector3 rotation;
    //Vector3 scale;
    //Vector3 dist;
    //float period;
    //Vector3 position = { 0.f, 0.f, 0.f };
    Vector3 startPoint = { 0.f, 0.f, 0.f };
    float width = 1.0f;
    float height = 1.0f;
    float depth = 1.0f;
    Vector4 offset = { 0.0f, 0.0f, 0.0f, 0.0f };
    Vector4 color = { 0.0f, 0.0f, 0.0f, 0.0f };

    UINT strides[1] = { 32 };
    UINT offsets[1] = { 0 };
};



