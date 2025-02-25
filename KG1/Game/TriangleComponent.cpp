#include "TriangleComponent.h"

bool TriangleComponent::Init(HWND hWnd, 
    Microsoft::WRL::ComPtr<ID3D11Device>& dev, 
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dContext, 
    std::shared_ptr<ConstantBuffer<ConstBuffVertexshader>> constBuffVS, 
    std::vector<DirectX::XMFLOAT4> points, 
    std::vector<int> indexes)
{
    window = hWnd;
    device = dev;
    context = dContext;
    renderPoints = points;
    this->indexes = indexes;

    if (!CompileShader())
        return false;


    if (!CreateVPBuffer())
        return false;

    constBuffVertexshader = constBuffVS;
    constBuffVertexshader->Initialize(device, context);
    constBuffVertexshader->data.offset = offset;
    constBuffVertexshader->ApplyChanges();

    return true;
}

bool TriangleComponent::CompileShader()
{
    vertexShaderByteCode = nullptr;
    ID3DBlob* errorVertexCode = nullptr;
    auto res = D3DCompileFromFile(
        L"./Shaders/MyVeryFirstShader.hlsl",
        nullptr /*macros*/,
        nullptr /*include*/,
        "VSMain",
        "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &vertexShaderByteCode,
        &errorVertexCode
    );

    if (FAILED(res)) {
        if (errorVertexCode) {
            char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

            std::cout << compileErrors << std::endl;
        }
        else
        {
            MessageBox(window, L"MyVeryFirstShader.hlsl", L"Missing Shader File", MB_OK);
        }

        return false;
    }

    D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

    ID3DBlob* errorPixelCode;
    res = D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl",
        Shader_Macros /*macros*/,
        nullptr /*include*/,
        "PSMain",
        "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &pixelShaderByteCode,
        &errorPixelCode
    );

    device->CreateVertexShader(
        vertexShaderByteCode->GetBufferPointer(),
        vertexShaderByteCode->GetBufferSize(),
        nullptr, 
        &vertexShader);

    device->CreatePixelShader(
        pixelShaderByteCode->GetBufferPointer(),
        pixelShaderByteCode->GetBufferSize(),
        nullptr, 
        &pixelShader);

    return true;
}

bool TriangleComponent::CreateVPBuffer()
{
    D3D11_INPUT_ELEMENT_DESC inputElements[] = {
        D3D11_INPUT_ELEMENT_DESC {
            "POSITION",
            0,
            DXGI_FORMAT_R32G32B32A32_FLOAT,
            0,
            0,
            D3D11_INPUT_PER_VERTEX_DATA,
            0},
        D3D11_INPUT_ELEMENT_DESC {
            "COLOR",
            0,
            DXGI_FORMAT_R32G32B32A32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0}
    };

    auto res = device->CreateInputLayout(
        inputElements,
        2,
        vertexShaderByteCode->GetBufferPointer(),
        vertexShaderByteCode->GetBufferSize(),
        &layout);

    if (FAILED(res))
        return false;


    // vertex buffer
    D3D11_BUFFER_DESC vertexBufDesc = {};
    vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufDesc.CPUAccessFlags = 0;
    vertexBufDesc.MiscFlags = 0;
    vertexBufDesc.StructureByteStride = 0;
    vertexBufDesc.ByteWidth = sizeof(renderPoints.front()) * std::size(renderPoints);

    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = &renderPoints.front();
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    res = device->CreateBuffer(&vertexBufDesc, &vertexData, &vertBuff);

    if (FAILED(res))
        return false;


    // index buffer
    D3D11_BUFFER_DESC indexBufDesc = {};
    indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufDesc.CPUAccessFlags = 0;
    indexBufDesc.MiscFlags = 0;
    indexBufDesc.StructureByteStride = 0;
    indexBufDesc.ByteWidth = sizeof(indexes.front()) * std::size(indexes);

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = &indexes.front();
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    res = device->CreateBuffer(&indexBufDesc, &indexData, &indBuff);

    if (FAILED(res))
        return false;


    // RASTERIZER
    CD3D11_RASTERIZER_DESC rastDesc = {};
    rastDesc.CullMode = D3D11_CULL_NONE;
    rastDesc.FillMode = D3D11_FILL_SOLID;

    res = device->CreateRasterizerState(&rastDesc, &rastState);

    if (FAILED(res))
        return false;

    return true;
}

bool TriangleComponent::Draw()
{
    context->RSSetState(rastState);

    context->IASetInputLayout(layout);
    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetIndexBuffer(indBuff, DXGI_FORMAT_R32_UINT, 0);
    context->IASetVertexBuffers(0, 1, &vertBuff, strides, offsets);

    context->VSSetShader(vertexShader, nullptr, 0);
    context->PSSetShader(pixelShader, nullptr, 0);

    return true;
}

void TriangleComponent::ClearData()
{
    renderPoints.clear();
    indexes.clear();

    if(layout)
        layout->Release();
    if(rastState)
        rastState->Release();
    if(indBuff)
        indBuff->Release();
    if(vertBuff)
        vertBuff->Release();
    if(vertexShader)
        vertexShader->Release();
    if(pixelShader)
        pixelShader->Release();
}

void TriangleComponent::UpdateOffset(float x, float y)
{
    offset.x += x;
    offset.y += y;

    if (constBuffVertexshader->Get())
    {
        constBuffVertexshader->data.offset = offset;
        constBuffVertexshader->ApplyChanges();
        context->VSSetConstantBuffers(0, 1, constBuffVertexshader->GetAddressOf());
    }
}
