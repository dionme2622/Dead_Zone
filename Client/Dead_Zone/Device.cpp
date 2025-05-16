#include "pch.h"
#include "Device.h"

void Device::Init()
{
    HRESULT hr;

#ifdef _DEBUG
    hr = D3D12GetDebugInterface(IID_PPV_ARGS(&_debugController));
    if (FAILED(hr)) { /* ����� �������̽� ���� ���� ó�� */ }
    _debugController->EnableDebugLayer();
#endif

    hr = CreateDXGIFactory1(IID_PPV_ARGS(&_dxgi));
    if (FAILED(hr)) { /* ���丮 ���� ���� ó�� */ }

    hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device));
    if (FAILED(hr))
    {
        // ��: hr == DXGI_ERROR_UNSUPPORTED �� ����̹��� D3D12 ������
        // �Ǵ� E_FAIL ��
        OutputDebugStringA(("D3D12CreateDevice ����: " + std::to_string(hr) + "\n").c_str());
    }
}
