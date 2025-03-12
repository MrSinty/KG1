#pragma once

#include "../../Support/DXInclude.h"
#include "../Transform/Transform.h"


class GameComponent
{
public:
    virtual ~GameComponent() = default;

    virtual bool Init(Transform* transform) = 0;
    virtual bool Draw() = 0;
    virtual void Update() = 0;

protected:
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
};

