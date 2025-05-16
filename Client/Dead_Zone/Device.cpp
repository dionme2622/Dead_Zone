#include "pch.h"
#include "Device.h"

void Device::Init()
{
    HRESULT hr;

#ifdef _DEBUG
    hr = D3D12GetDebugInterface(IID_PPV_ARGS(&_debugController));
    if (FAILED(hr)) { /* 디버그 인터페이스 생성 실패 처리 */ }
    _debugController->EnableDebugLayer();
#endif

    hr = CreateDXGIFactory1(IID_PPV_ARGS(&_dxgi));
    if (FAILED(hr)) { /* 팩토리 생성 실패 처리 */ }

    hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device));
    if (FAILED(hr))
    {
        // 예: hr == DXGI_ERROR_UNSUPPORTED 시 드라이버가 D3D12 미지원
        // 또는 E_FAIL 등
        OutputDebugStringA(("D3D12CreateDevice 실패: " + std::to_string(hr) + "\n").c_str());
    }
}
