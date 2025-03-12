#include "Transform.h"

Transform::Transform()
{
    scale = Vector3(1);
    UpdateWorldMatrix();
}

void Transform::Init(Transform* pparent, Transform* ttransformRotationCenter)
{
    parent = pparent;
    transformRotationCenter = ttransformRotationCenter;
}

void Transform::UpdateWorldMatrix()
{
    worldMatrix =
        Matrix::CreateScale(scale)
        * Matrix::CreateFromYawPitchRoll(rotation)
        * Matrix::CreateTranslation(position)
        * (parent == nullptr ? Matrix::Identity : parent->WorldMatrix());
}

void Transform::SetSpeed(const Vector3& ooffset, const Vector3& aaxisAroundCenter, float sspeedAroundCenter, const Vector3& aaxisSelf, float sspeedSelfAxis)
{
    offset = ooffset;
    axisAroundCenter = aaxisAroundCenter;
    speedAroundCenter = sspeedAroundCenter;
    axisSelf = aaxisSelf;
    speedSelfAxis = sspeedSelfAxis;
}

void Transform::Update(float delta)
{
    if (transformRotationCenter != nullptr) UpdateAroundCenter(delta);
    UpdateSelf(delta);
    UpdateWorldMatrix();
}

void Transform::UpdateAroundCenter(float delta)
{
    angleAroundCenter += speedAroundCenter * delta;
    const auto rotationAroundCenter = Matrix::CreateFromAxisAngle(axisAroundCenter, angleAroundCenter);
    position = transformRotationCenter->Position()
        + Vector3::Transform(offset, rotationAroundCenter);
}

void Transform::UpdateSelf(float delta)
{
    const auto rotationAroundSelf = Matrix::CreateFromAxisAngle(axisSelf, speedSelfAxis * delta);
    auto rotated = worldMatrix * rotationAroundSelf;
    Vector3 scale, translation;
    DirectX::SimpleMath::Quaternion newRotation;
    rotated.Decompose(scale, newRotation, translation);
    rotation = newRotation.ToEuler();
}

Transform* Transform::Parent() const
{
    return parent;
}

Matrix& Transform::WorldMatrix()
{
    return worldMatrix;
}

Vector3& Transform::Position()
{
    return position;
}

Vector3& Transform::Rotation()
{
    return rotation;
}

Vector3& Transform::Scale()
{
    return scale;
}