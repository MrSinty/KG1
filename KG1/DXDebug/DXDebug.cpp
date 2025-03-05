#include "DXDebug.h"

bool DXDebug::Init(Microsoft::WRL::ComPtr<ID3D11Device>& device)
{
#ifdef _DEBUG
    // Init DXGI Debug
    if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&m_dxgiDebug))))
    {
        m_dxgiDebug->EnableLeakTrackingForThread();
    }

    if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&m_d3d11Debug))))
    {
        return true;
    }
    else
        return false;


#endif

    return true;
}

void DXDebug::Shutdown()
{
#ifdef _DEBUG
    if (m_dxgiDebug)
    {
        OutputDebugStringW(L"DXGI Reports living device objects:\n");
        m_dxgiDebug->ReportLiveObjects(
            DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL)
        );
        m_dxgiDebug->Release();
    }

    if (m_d3d11Debug)
    {
        OutputDebugStringW(L"D3D11 Debug reports living device objects:\n");
        m_d3d11Debug->ReportLiveDeviceObjects(D3D11_RLDO_FLAGS(D3D11_RLDO_DETAIL | D3D11_RLDO_IGNORE_INTERNAL));
        m_d3d11Debug->Release();
    }
#endif
}
