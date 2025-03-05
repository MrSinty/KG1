#include "GameComponent.h"




//
//bool GameComponent::Init(HWND hWnd, Microsoft::WRL::ComPtr<ID3D11Device>& dev, 
//    Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dContext, 
//    std::vector<points_indexes> data)
//{
//    constBuffVS = std::make_shared<ConstantBuffer<ConstBuffVertexshader>>();
//    constBuffVS->Initialize(dev, dContext);
//    DirectX::XMFLOAT4 col = DirectX::XMFLOAT4(1.0f, .5f, .0f, 1.0f);
//    constBuffVS->data.color = col;
//
//    TriangleComponent triangle = TriangleComponent();
//
//    if (!triangle.Init(hWnd, dev, dContext, constBuffVS, item.points, item.indexes))
//        return false;
//    triangleBuff.push_back(triangle);
//
//    return true;
//}
//
//bool GameComponent::Draw()
//{
//    for (auto& triangle : triangleBuff)
//    {
//        if (!triangle.Draw())
//            return false;
//    }
//
//    return true;
//}
//
//void GameComponent::UpdateTriangleOffset(float x, float y)
//{
//    for (auto& triangle : triangleBuff)
//    {
//        triangle.UpdateOffset(x, y);
//    }
//}
//
//void GameComponent::Shutdown()
//{
//    for (auto& triangle : triangleBuff)
//    {
//        triangle.ClearData();
//    }
//}
//