#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<vector>
#include<string>
#include <wrl.h>
#include "WindowsApp.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")

using namespace std;
using namespace Microsoft::WRL;

class DirectXInitialize
{
public:
	HRESULT result;

	ComPtr <ID3D12Device> device;
	ComPtr <IDXGIFactory6> dxgiFactory;
	ComPtr <IDXGISwapChain4> swapChain;
	ComPtr <ID3D12CommandAllocator> commandAllocator;
	ComPtr <ID3D12GraphicsCommandList> commandList;
	ComPtr <ID3D12CommandQueue> commandQueue;
	ComPtr <ID3D12DescriptorHeap> rtvHeap;

	//�R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

	// �o�b�N�o�b�t�@
	std::vector<ID3D12Resource*> backBuffers;

	D3D12_RESOURCE_DESC depthResourceDesc{};

	D3D12_HEAP_PROPERTIES depthHeapProp{};

	D3D12_CLEAR_VALUE depthClearValue{};

	ID3D12Resource* depthBuff = nullptr;

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};

	ID3D12DescriptorHeap* dsvHeap = nullptr;

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

	D3D12_RESOURCE_BARRIER barrierDesc{};

	D3D12_VIEWPORT viewport{};

	D3D12_RECT scissorRect{};

	// �t�F���X�̐���
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;

	void createDX(HWND hwnd);

	void PreDraw();

	void PostDraw();

};