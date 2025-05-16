#include "pch.h"
#include "DebugRenderer.h"
#include "Engine.h"
#include "Shader.h"
#include "Resources.h"
#include "Camera.h"
#include "ConstantBuffer.h"
#include "Transform.h"
void DebugRenderer::Init(ComPtr<ID3D12Device> device, UINT maxLines) {
    if (_initialized) return;
    _maxLines = maxLines;
    UINT vbSize = sizeof(DebugVertex) * _maxLines * 2;

    // Default heap (GPU local)
    D3D12_RESOURCE_DESC desc = {};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Alignment = 0;
    desc.Width = vbSize;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

    CD3DX12_HEAP_PROPERTIES defaultHeapProps(D3D12_HEAP_TYPE_DEFAULT);
    device->CreateCommittedResource(
        &defaultHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&_vertexBuffer));

    // Upload heap
    CD3DX12_HEAP_PROPERTIES uploadProps(D3D12_HEAP_TYPE_UPLOAD);
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;
    device->CreateCommittedResource(
        &uploadProps,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&_vertexBufferUpload));

    // Vertex buffer view binds default heap
    _vbView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
    _vbView.SizeInBytes = vbSize;
    _vbView.StrideInBytes = sizeof(DebugVertex);

    _initialized = true;
    _shader = GET_SINGLE(Resources)->Get<Shader>(L"Collider");
}

void DebugRenderer::Shutdown() {
    _vertexBuffer.Reset();
    _vertexBufferUpload.Reset();
    _lines.clear();
    _initialized = false;
}

void DebugRenderer::Clear() {
    _lines.clear();
}

void DebugRenderer::AddLine(const XMFLOAT3& p0, const XMFLOAT3& p1, const XMFLOAT4& color) {
    if (_lines.size() + 2 > _maxLines * 2) return;
    _lines.push_back({ p0 });
    _lines.push_back({ p1 });
}

void DebugRenderer::AddBox(const BoundingBox& b, const XMMATRIX& world, const XMFLOAT4& color) {
    XMFLOAT3 c = b.Center, e = b.Extents;
    XMFLOAT3 corners[8] = {
        {c.x - e.x,c.y - e.y,c.z - e.z}, {c.x + e.x,c.y - e.y,c.z - e.z},
        {c.x + e.x,c.y + e.y,c.z - e.z}, {c.x - e.x,c.y + e.y,c.z - e.z},
        {c.x - e.x,c.y - e.y,c.z + e.z}, {c.x + e.x,c.y - e.y,c.z + e.z},
        {c.x + e.x,c.y + e.y,c.z + e.z}, {c.x - e.x,c.y + e.y,c.z + e.z}
    };
    static const int edges[24] = { 0,1,1,2,2,3,3,0,4,5,5,6,6,7,7,4,0,4,1,5,2,6,3,7 };
    for (int i = 0; i < 24; i += 2) {
        XMVECTOR p0 = XMVector3Transform(XMLoadFloat3(&corners[edges[i]]), world);
        XMVECTOR p1 = XMVector3Transform(XMLoadFloat3(&corners[edges[i + 1]]), world);
        XMFLOAT3 fp0, fp1;
        XMStoreFloat3(&fp0, p0); XMStoreFloat3(&fp1, p1);
        AddLine(fp0, fp1, color);
    }
}

void DebugRenderer::AddCapsule(float radius, float height, const XMMATRIX& world, const XMFLOAT4& color) {
    const int seg = 16;
    float half = height * 0.5f;
    for (int i = 0; i < seg; ++i) {
        float a0 = XM_2PI * i / seg;
        float a1 = XM_2PI * (i + 1) / seg;
        XMFLOAT3 p0{ radius * cosf(a0),-half, radius * sinf(a0) };
        XMFLOAT3 p1{ radius * cosf(a1),-half, radius * sinf(a1) };
        XMFLOAT3 p2{ radius * cosf(a0),+half, radius * sinf(a0) };
        XMFLOAT3 p3{ radius * cosf(a1),+half, radius * sinf(a1) };
        AddLine({ DirectX::XMVector3TransformCoord(XMLoadFloat3(&p0), world).m128_f32[0],
                 DirectX::XMVector3TransformCoord(XMLoadFloat3(&p0), world).m128_f32[1],
                 DirectX::XMVector3TransformCoord(XMLoadFloat3(&p0), world).m128_f32[2] },
            { DirectX::XMVector3TransformCoord(XMLoadFloat3(&p1), world).m128_f32[0],
             DirectX::XMVector3TransformCoord(XMLoadFloat3(&p1), world).m128_f32[1],
             DirectX::XMVector3TransformCoord(XMLoadFloat3(&p1), world).m128_f32[2] }, color);
        AddLine({ DirectX::XMVector3TransformCoord(XMLoadFloat3(&p2), world).m128_f32[0],
                 DirectX::XMVector3TransformCoord(XMLoadFloat3(&p2), world).m128_f32[1],
                 DirectX::XMVector3TransformCoord(XMLoadFloat3(&p2), world).m128_f32[2] },
            { DirectX::XMVector3TransformCoord(XMLoadFloat3(&p3), world).m128_f32[0],
             DirectX::XMVector3TransformCoord(XMLoadFloat3(&p3), world).m128_f32[1],
             DirectX::XMVector3TransformCoord(XMLoadFloat3(&p3), world).m128_f32[2] }, color);
        AddLine({ DirectX::XMVector3TransformCoord(XMLoadFloat3(&p0), world).m128_f32[0],
                 DirectX::XMVector3TransformCoord(XMLoadFloat3(&p0), world).m128_f32[1],
                 DirectX::XMVector3TransformCoord(XMLoadFloat3(&p0), world).m128_f32[2] },
            { DirectX::XMVector3TransformCoord(XMLoadFloat3(&p2), world).m128_f32[0],
             DirectX::XMVector3TransformCoord(XMLoadFloat3(&p2), world).m128_f32[1],
             DirectX::XMVector3TransformCoord(XMLoadFloat3(&p2), world).m128_f32[2] }, color);
    }
}

void DebugRenderer::Flush() {
    if (!_initialized || _lines.empty()) return;
    // 1) Upload buffer에 복사
    void* p;
    CD3DX12_RANGE readRange(0, 0);
    _vertexBufferUpload->Map(0, &readRange, &p);
    memcpy(p, _lines.data(), sizeof(DebugVertex) * _lines.size());
    _vertexBufferUpload->Unmap(0, nullptr);

    // 2) Copy to default heap
    GRAPHICS_CMD_LIST->CopyBufferRegion(
        _vertexBuffer.Get(), 0,
        _vertexBufferUpload.Get(), 0,
        sizeof(DebugVertex) * _lines.size());

    _shader->Update();


    // 0) 현재 디버그용 월드 행렬(Identity 혹은 필요 시 합산)을 Push
    TransformParams tp = {};
    tp.matWorld = XMMatrixIdentity();           // 이미 CPU에서 world 변환이 끝난 경우
    tp.matView = Camera::S_MatView;
    tp.matProjection = Camera::S_MatProjection;
    tp.matWV = tp.matWorld * tp.matView;
    tp.matWVP = tp.matWorld * tp.matView * tp.matProjection;
    CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushGraphicsData(&tp, sizeof(tp));

    // 3) Draw
    GRAPHICS_CMD_LIST->IASetVertexBuffers(0, 1, &_vbView);
    GRAPHICS_CMD_LIST->DrawInstanced((UINT)_lines.size(), 1, 0, 0);
}
