#pragma once

#include "../Support/DXInclude.h"
#include "DirectXCollision.h"
#include "SimpleMath.h"
#include "Components/TriangleComponent.h"

class Planet
{
public:
    Planet(Microsoft::WRL::ComPtr<ID3D11Device>& dev,
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dContext);
    void Init(Vector3& position, Vector3& scale, Planet* parent, Transform* centerTransform);
    void Update(float deltaTime);
    void Draw();

    TriangleComponent* mesh = nullptr;
    Transform* transform = nullptr;
    Planet* parent = nullptr;
    DirectX::BoundingBox collider;
};

