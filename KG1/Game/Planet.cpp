#include "Planet.h"

Planet::Planet(Microsoft::WRL::ComPtr<ID3D11Device>& dev, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dContext)
{
    mesh = new TriangleComponent(dev, dContext);
    transform = new Transform();
}

void Planet::Init(Vector3& position, Vector3& scale, Planet* parent, Transform* centerTransform)
{
    transform->Position() = position;
    transform->Scale() = scale;
    transform->UpdateWorldMatrix();
    collider.Center = position;
    collider.Extents = scale / 3;

    if(parent)
        transform->Init(parent->transform, centerTransform);
    else
        transform->Init(nullptr, centerTransform);
    mesh->Init(transform);
}

void Planet::Update(float deltaTime)
{
    transform->Update(deltaTime);
    mesh->Update();

    collider.Center = transform->Position();
}

void Planet::Draw()
{
    mesh->Draw();
}
