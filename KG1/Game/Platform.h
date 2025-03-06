#pragma once
#include "DirectXCollision.h"
#include "SimpleMath.h"
#include "Components/TriangleComponent.h"

class Platform
{
public:
    Platform(Microsoft::WRL::ComPtr<ID3D11Device>& dev,
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dContext);

    void Init(float startX, float startY, float width, float height);
    void Update(float deltaTime);
    bool Draw();
    void UpdateOffset(float x, float y);
    void KeyDown(bool isDown);
    void IsUpKey(bool isUp);
    inline DirectX::BoundingBox GetCollider() const { return collider; }

    Vector2 startPos = { 0.f, 0.f };
    float width = 0.5f;
    float height = 0.5f;

    bool isKeyDown = false;
    float speed = 0.5f;
    Vector2 direction = { 0.0f, 0.0f };

    TriangleComponent* mesh = nullptr;
    DirectX::BoundingBox collider;
};

