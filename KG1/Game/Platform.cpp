#include "Platform.h"


Platform::Platform(Microsoft::WRL::ComPtr<ID3D11Device>& dev, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dContext)
{
    mesh = new TriangleComponent(dev, dContext);
}

void Platform::Init(float startX, float startY, float wwidth, float hheight)
{
    startPos = { startX, startY };
    width = wwidth / 2;
    height = hheight / 2;

    mesh->SetLocationAndForm(startPos, wwidth, hheight);
    mesh->SetColor(1.0f, 1.0f, 0.0f, 1.0f);
    mesh->Init();

    collider.Center = { startPos.x, startPos.y, 0.0f };
    collider.Extents = { width, height, 0.0f };
}

void Platform::Update()
{
    mesh->Update();
}

bool Platform::Draw()
{
    return mesh->Draw();
}

void Platform::UpdateOffset(float x, float y)
{
    collider.Center.x += x;
    collider.Center.y += y;
    mesh->UpdateOffset(x, y);
}
