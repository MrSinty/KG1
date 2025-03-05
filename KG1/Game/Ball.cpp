#include "Ball.h"

Ball::Ball(Microsoft::WRL::ComPtr<ID3D11Device>& dev, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dContext)
{
    mesh = new TriangleComponent(dev, dContext);
}

void Ball::Init(float startX, float startY, float wwidth, float hheight)
{
    startPos = { startX, startY };
    width = wwidth / 2;
    height = hheight / 2;

    mesh->SetLocationAndForm(startPos, wwidth, hheight);
    mesh->Init();

    collider.Center = { startPos.x, startPos.y, 0.0f };
    collider.Extents = { width, height, 0.0f };
}

    void Ball::Update(float deltaTime)
    {
        if (collider.Center.x < -1.f || collider.Center.x > 1.f)
        {
            SpawnOnStart();
        }

        mesh->Update();

        Vector2 offset = direction * speed * deltaTime;
        UpdateOffset(offset.x, offset.y);

        if (wasColliding && !isColliding)
        {
            wasColliding = false;
        }
        isColliding = false;

    }

    bool Ball::Draw()
    {
        return mesh->Draw();
    }

    void Ball::UpdateOffset(float x, float y)
    {
        mesh->UpdateOffset(x, y);
        collider.Center.x += x;
        collider.Center.y += y;
    }

    void Ball::SetOffset(float x, float y)
    {
        mesh->SetOffset(x, y);
        collider.Center.x = x;
        collider.Center.y = y;
    }

    void Ball::ChangeDirection()
    {
        direction *= -1;
    }

    void Ball::ChangeDirectionAfterPlatform(Vector3 otherColCenter, bool isRightPlatform)
    {
        float xDir = isRightPlatform ? 1.0f : -1.0f;

        if (collider.Center.y > otherColCenter.y + centerArea/2)
        {
            direction = { xDir, -1.0f };
        }
        else if (collider.Center.y < otherColCenter.y - centerArea/2)
        {
            direction = { xDir, 1.0f };
        }
        else
        {
            direction = { xDir, 0.f };
        }

        speed += 0.05f;
        direction.Normalize(direction);
        ChangeDirection();
    }

    void Ball::ChangeDirectionAfterWall()
    {
        direction.y *= -1;
    }

    void Ball::SpawnOnStart()
    {
        speed = 0.5f;
        direction = { -1.0f, 0.0f };
        SetOffset(0.f, 0.f);
    }