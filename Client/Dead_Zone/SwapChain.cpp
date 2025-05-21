#include "pch.h"
#include "SwapChain.h"
#include "EnginePch.h"
#include "Engine.h"


void SwapChain::Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	CreateSwapChain(info, dxgi, cmdQueue);
}

void SwapChain::Present()
{
	// Present the frame.
	_swapChain->Present(0, 0);
}

void SwapChain::SwapIndex()
{
	_backBufferIndex = (_backBufferIndex + 1) % SWAP_CHAIN_BUFFER_COUNT;
}

void SwapChain::CreateSwapChain(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	// ������ ���� ���� ������
	_swapChain.Reset();

    _originalWidth = info.width;
    _originalHeight = info.height;

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = static_cast<uint32>(info.width); // ������ �ػ� �ʺ�
	sd.BufferDesc.Height = static_cast<uint32>(info.height); // ������ �ػ� ����
	sd.BufferDesc.RefreshRate.Numerator = 60; // ȭ�� ���� ����
	sd.BufferDesc.RefreshRate.Denominator = 1; // ȭ�� ���� ����
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // ������ ���÷��� ����
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1; // ��Ƽ ���ø� OFF
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // �ĸ� ���ۿ� �������� �� 
	sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT; // ����+�ĸ� ����
	sd.OutputWindow = info.hwnd;
	sd.Windowed = info.windowed;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // ���� �ĸ� ���� ��ü �� ���� ������ ���� ����
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	dxgi->CreateSwapChain(cmdQueue.Get(), &sd, &_swapChain);

}

void SwapChain::ChangeSwapChainState(WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	// 1. ���� ��� ���
	info.windowed = !info.windowed;

	// 2. ��üȭ��/â��� ��ȯ
	_swapChain->SetFullscreenState(!info.windowed, nullptr);

	

	// 3. â��� ���� �� ������ ��Ÿ��/ũ�� ����
	if (info.windowed)
	{
		SetWindowLongPtr(info.hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		RECT wr = { 0,0, static_cast<LONG>(info.width), static_cast<LONG>(info.height) };
		AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
		SetWindowPos(info.hwnd, nullptr, 0, 0,
			wr.right - wr.left, wr.bottom - wr.top,
			SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}

	_swapChain->ResizeBuffers(SWAP_CHAIN_BUFFER_COUNT, info.width, info.height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
}

