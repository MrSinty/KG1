#include "Platform.h"


Platform::Platform(Microsoft::WRL::ComPtr<ID3D11Device>& dev, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dContext)
{
    mesh = new TriangleComponent(dev, dContext);
}

void Platform::Init(float startX, float startY, float startZ, float wwidth, float hheight, float ddepth)
{
    startPos = { startX, startY, startZ };
    width = wwidth / 2;
    height = hheight / 2;
    depth = ddepth / 2;

    mesh->SetLocationAndForm(startPos, wwidth, hheight, ddepth);
    mesh->Init();

    collider.Center = { startPos.x, startPos.y, startPos.z };
    collider.Extents = { width, height, depth };
}

void Platform::Update(float deltaTime)
{
    Vector2 offset = direction * speed * deltaTime;
    UpdateOffset(offset.x, offset.y);

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

void Platform::KeyDown(bool isDown)
{
    if (isDown)
        isKeyDown = true;
    else
        isKeyDown = false;
}

void Platform::KeyPressed(int keyNum) // 0 - Up, 1 - Down, 2 - Right, 3 - Left
{
    if (isKeyDown)
    {
        switch (keyNum)
        {
        case 0:
            direction += { 0.0f, 1.0f };
            break;
        case 1:
            direction += { 0.0f, -1.0f };
            break;
        case 2:
            direction += { 1.0f, 0.0f };
            break;
        case 3:
            direction += { -1.0f, 0.0f };
            break;
        }
        direction.Normalize(direction);
    }
    else
        direction = { 0.f, 0.f };
}