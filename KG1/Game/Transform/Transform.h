#pragma once
#include "../../Support/DXInclude.h"

using namespace DirectX::SimpleMath;

class Transform
{
public:
    Transform();

    void Init(Transform* parent, Transform* transformRotationCenter);
    void UpdateWorldMatrix();

    void SetSpeed(
        const Vector3& offset, const Vector3& axisAroundCenter, float speedAroundCenter,
        const Vector3& axisSelf, float speedSelfAxis);
    void Update(float delta);

    Transform* Parent() const;

    Vector3& Position();
    Vector3& Rotation();
    Vector3& Scale();
    Matrix& WorldMatrix();

private:
    void UpdateAroundCenter(float delta);
    void UpdateSelf(float delta);

    Transform* parent = nullptr;

    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    Matrix worldMatrix;

    Transform* transformRotationCenter = nullptr;

    Vector3 offset;
    Vector3 axisAroundCenter;
    float speedAroundCenter = 0;
    float angleAroundCenter = 0;
    Vector3 axisSelf;
    float speedSelfAxis = 0;
};

