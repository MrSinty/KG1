#include "TriangleComponent.h"

TriangleComponent::TriangleComponent(Microsoft::WRL::ComPtr<ID3D11Device>& dev, 
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dContext)
{
    device = dev;
    deviceContext = dContext;
}

TriangleComponent::~TriangleComponent()
{
    ClearData();
}

bool TriangleComponent::Init()
{
    if (!CompileShader())
        return false;

    if (!CreateVPBuffer())
        return false;

    constBuff.Initialize(device, deviceContext);

    // RASTERIZER
    CD3D11_RASTERIZER_DESC rastDesc = {};
    rastDesc.CullMode = D3D11_CULL_FRONT;
    rastDesc.FillMode = D3D11_FILL_SOLID;

    auto res = device->CreateRasterizerState(&rastDesc, &rastState);

    if (FAILED(res))
        return false;

    return true;
}

bool TriangleComponent::Draw()
{
    deviceContext->RSSetState(rastState);

    deviceContext->IASetInputLayout(layout);
    deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    deviceContext->IASetIndexBuffer(indBuff, DXGI_FORMAT_R32_UINT, 0);
    deviceContext->IASetVertexBuffers(0, 1, &vertBuff, strides, offsets);

    deviceContext->VSSetShader(vertexShader, nullptr, 0);
    deviceContext->PSSetShader(pixelShader, nullptr, 0);

    deviceContext->VSSetConstantBuffers(0, 1, constBuff.GetAddressOf());

    deviceContext->DrawIndexed(indices.size(), 0, 0);

    return true;
}

void TriangleComponent::Update()
{
    if (constBuff.Get())
    {
        constBuff.data.mWorld = World.Transpose();
        constBuff.data.mView = View.Transpose();
        constBuff.data.mProjection = Proj.Transpose();
        constBuff.data.offset = offset;
        constBuff.data.color = color;
        constBuff.ApplyChanges();
    }
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
    indexBufDesc.ByteWidth = sizeof(indices.front()) * std::size(indices);

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = &indices.front();
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    res = device->CreateBuffer(&indexBufDesc, &indexData, &indBuff);

    if (FAILED(res))
        return false;

    return true;
}

void TriangleComponent::SetLocationAndForm(Vector3 center, float wwidth, float hheight, float ddepth)
{
    centerPoint = center;
    width = wwidth;
    height = hheight;
    depth = ddepth;

    renderPoints[0].x = centerPoint.x - width / 2;
    renderPoints[0].y = centerPoint.y + height / 2;
    renderPoints[0].z = centerPoint.z - depth / 2;

    renderPoints[2].x = centerPoint.x + width / 2;
    renderPoints[2].y = centerPoint.y + height / 2;
    renderPoints[2].z = centerPoint.z - depth / 2;

    renderPoints[4].x = centerPoint.x + width / 2;
    renderPoints[4].y = centerPoint.y + height / 2;
    renderPoints[4].z = centerPoint.z + depth / 2;

    renderPoints[6].x = centerPoint.x - width / 2;
    renderPoints[6].y = centerPoint.y + height / 2;
    renderPoints[6].z = centerPoint.z + depth / 2;

    renderPoints[8].x = centerPoint.x - width / 2;
    renderPoints[8].y = centerPoint.y - height / 2;
    renderPoints[8].z = centerPoint.z - depth / 2;

    renderPoints[10].x = centerPoint.x + width / 2;
    renderPoints[10].y = centerPoint.y - height / 2;
    renderPoints[10].z = centerPoint.z - depth / 2;

    renderPoints[12].x = centerPoint.x + width / 2;
    renderPoints[12].y = centerPoint.y - height / 2;
    renderPoints[12].z = centerPoint.z + depth / 2;

    renderPoints[14].x = centerPoint.x - width / 2;
    renderPoints[14].y = centerPoint.y - height / 2;
    renderPoints[14].z = centerPoint.z + depth / 2;
}

void TriangleComponent::ChangeScale(Vector3 scale)
{
    LocalMat *= Matrix::CreateScale(scale);
}

void TriangleComponent::ChangeRotX(float radians)
{
    LocalMat *= Matrix::CreateRotationX(radians);
}

void TriangleComponent::ChangeRotY(float radians)
{
    LocalMat *= Matrix::CreateRotationY(radians);
}

void TriangleComponent::ChangeRotZ(float radians)
{
    LocalMat *= Matrix::CreateRotationZ(radians);
}

void TriangleComponent::ChangeTranslation(Vector3 distance)
{
    LocalMat *= Matrix::CreateTranslation(distance);
}

void TriangleComponent::RotateAroundPoint(Vector3 pivot, float angle)
{
    Matrix translationToPivot = Matrix::CreateTranslation(-pivot);

    Matrix rotation = Matrix::CreateRotationY(angle);

    Matrix translationBack = Matrix::CreateTranslation(pivot);

    LocalMat *= translationToPivot * rotation * translationBack;
}

void TriangleComponent::UpdateTransform()
{
    World = LocalMat;
    //centerPoint = DirectX::XMVector3Transform(centerPoint, LocalMat);
    LocalMat = Matrix::Identity;
}

void TriangleComponent::UpdateTransform(const Matrix& parentWorld)
{
    World = LocalMat * parentWorld;
    LocalMat = Matrix::Identity;
}

void TriangleComponent::SetMatricies(Matrix& wrld, Matrix& view, Matrix& proj)
{
    World = wrld;
    View = view;
    Proj = proj;
}

void TriangleComponent::SetMatricies(Matrix& view, Matrix& proj)
{
    View = view;
    Proj = proj;
}

void TriangleComponent::SetVertexCoordinates(std::vector<Vector3>& points)
{
    for (int i = 0; i < renderPoints.size(); i += 2)
    {
        renderPoints[i].x = points[i / 2].x;
        renderPoints[i].y = points[i / 2].y;
        renderPoints[i].z = points[i / 2].z;
    }
}

void TriangleComponent::SetOffset(float x, float y)
{
    offset.x = x;
    offset.y = y;
}

void TriangleComponent::SetColor(float r, float g, float b, float a)
{
    color = Vector4( r, g, b, a );
}

void TriangleComponent::UpdateOffset(float x, float y)
{
    offset.x += x;
    offset.y += y;
}

Vector4 TriangleComponent::GetLocationAndForm()
{
    Vector4 vec;
    vec.x = centerPoint.x;
    vec.y = centerPoint.y;
    vec.z = width;
    vec.w = height;
    return vec;
}

DirectX::SimpleMath::Matrix TriangleComponent::GetLocalMatrix()
{
    return LocalMat;
}

DirectX::SimpleMath::Vector3 TriangleComponent::GetCenterPoint()
{
    return centerPoint;
}

void TriangleComponent::ClearData()
{
    renderPoints.clear();
    indices.clear();

    if (layout)
        layout->Release();
    if (rastState)
        rastState->Release();
    if (indBuff)
        indBuff->Release();
    if (vertBuff)
        vertBuff->Release();
    if (vertexShader)
        vertexShader->Release();
    if (pixelShader)
        pixelShader->Release();
}

