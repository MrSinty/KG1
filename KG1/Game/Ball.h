#pragma once
#include "DirectXCollision.h"
#include "SimpleMath.h"
#include "Components/TriangleComponent.h"

class Ball
{
public:
    Ball(Microsoft::WRL::ComPtr<ID3D11Device>& dev, 
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dContext);

    void Init(float startX, float startY, float width, float height);
    void Update(float deltaTime);
    bool Draw();
    void UpdateOffset(float x, float y);
    void SetOffset(float x, float y);
    void ChangeDirection();
    void ChangeDirectionAfterPlatform(Vector3 otherColCenter, bool isRightPlatform);
    void ChangeDirectionAfterWall();
    void SpawnOnStart();
    inline bool CheckCollision(DirectX::BoundingBox& otherCollider) const { return collider.Intersects(otherCollider); }
    inline DirectX::BoundingBox GetCollider() const { return collider; }

    Vector2 startPos = { 0.f, 0.f };
    float width = 0.5f;
    float height = 0.5f;

    TriangleComponent* mesh;
    DirectX::BoundingBox collider;

    float centerArea = 0.2f;
    float speed = 0.5f;
    Vector2 direction{ -1.0f, 0.0f };
    bool isColliding = false;
    bool wasColliding = false;
};

