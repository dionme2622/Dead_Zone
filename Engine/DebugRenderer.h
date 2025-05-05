#pragma once
struct DebugVertex {
    XMFLOAT3 pos;
};

class Shader;

class DebugRenderer {
public:
    DECLARE_SINGLE(DebugRenderer);


    // �ʱ�ȭ: device�� �ִ� ���� ���� ����
    void Init(ComPtr<ID3D12Device> device, UINT maxLines);
    void Shutdown();

    // ����� ���� �߰�
    void AddLine(const DirectX::XMFLOAT3& p0,
        const DirectX::XMFLOAT3& p1,
        const DirectX::XMFLOAT4& color);
    void AddBox(const BoundingBox& b,
        const DirectX::XMMATRIX& world,
        const DirectX::XMFLOAT4& color);
    void AddCapsule(float radius,
        float height,
        const DirectX::XMMATRIX& world,
        const DirectX::XMFLOAT4& color);

    // ������: Flush ȣ�� �� ���ε�->����Ʈ ���� �� Draw
    void Flush();
    void Clear();

private:
    std::vector<DebugVertex>               _lines;
    Microsoft::WRL::ComPtr<ID3D12Resource> _vertexBuffer;        // Default heap
    Microsoft::WRL::ComPtr<ID3D12Resource> _vertexBufferUpload;  // Upload heap
    D3D12_VERTEX_BUFFER_VIEW               _vbView = {};
    UINT                                   _maxLines = 0;
    bool                                   _initialized = false;

    shared_ptr<Shader>                     _shader = nullptr;
};
