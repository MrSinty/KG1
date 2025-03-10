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

    //mesh->SetLocationAndForm(startPos, wwidth, hheight);
    mesh->Init();

    collider.Center = { startPos.x, startPos.y, 0.0f };
    collider.Extents = { width, height, 0.0f };
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