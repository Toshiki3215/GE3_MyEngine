// DirectXInitialize.h
// DirectXの初期化処理を管理している

#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "WindowsApp.h"
#include <vector>

class DirectXInitialize
{
public://メンバ関数

	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//-----初期化-----//
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="winApp"></param>
	void Initialize(WinApp* winApp);

	/// <summary>
	/// デバイスの初期化
	/// </summary>
	void InitializeDevice();

	/// <summary>
	/// コマンドリストの初期化
	/// </summary>
	void InitializeCommand();

	/// <summary>
	/// スワップチェーンの初期化
	/// </summary>
	void InitializeSwapchain();

	/// <summary>
	/// レンダーターゲットビューの初期化
	/// </summary>
	void InitializeRenderTargetView();

	/// <summary>
	/// デプスバッファの初期化
	/// </summary>
	void InitializeDepthBuffer();

	/// <summary>
	/// フェンスの初期化
	/// </summary>
	void InitializeFence();

	//描画前処理
	void PreDraw();

	//描画後処理
	void PostDraw();

	//デバイス取得
	ID3D12Device* GetDevice() const { return device.Get(); }

	//コマンドリスト取得
	ID3D12GraphicsCommandList* GetCommandList()const { return commandList.Get(); }

	//バックバッファの数を取得
	size_t GetBackBufferCount() const { return backBuffers.size(); }

private:

	//DirectX12デバイス
	ComPtr<ID3D12Device> device;

	//DXGIファクトリ
	ComPtr<IDXGIFactory7> dxgiFactory;

	//バックバッファ
	std::vector<ComPtr<ID3D12Resource>> backBuffers;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	D3D12_RESOURCE_DESC depthResourceDesc{};

	//WindowsAPI
	WinApp* winApp_ = nullptr;

	HRESULT result;
	ComPtr<IDXGISwapChain4> swapChain;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	ComPtr<ID3D12Fence> fence;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	D3D12_RESOURCE_BARRIER barrierDesc{};
	UINT64 fenceVal = 0;
	ComPtr<ID3D12Resource> depthBuff;
};