#pragma once
#include "../Support/DXInclude.h"

class DXDebug
{
public:
    bool Init(Microsoft::WRL::ComPtr<ID3D11Device>&);
    void Shutdown();

private:
#ifdef _DEBUG
    Microsoft::WRL::ComPtr<IDXGIDebug1> m_dxgiDebug;
    Microsoft::WRL::ComPtr<ID3D11Debug> m_d3d11Debug;
#endif

    //Singleton
public:
    DXDebug(const DXDebug&) = delete;
    DXDebug& operator=(const DXDebug&) = delete;

    inline static DXDebug& Get()
    {
        static DXDebug instance;
        return instance;
    }

private:
    DXDebug() = default;
};

