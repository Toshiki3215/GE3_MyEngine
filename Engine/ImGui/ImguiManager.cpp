#include "ImguiManager.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include <cassert>
#include <vector>

void ImguiManager::Initialize(WinApp* winApp, DirectXInitialize* dxInit)
{
	dxInit_ = dxInit;

	//Imguiのコンテキストを生成
	ImGui::CreateContext();

	//Imguiのスタイルを設定
	ImGui::StyleColorsLight();

	ImGui_ImplWin32_Init(winApp->GetHwnd());

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	//デスクリプタヒープ生成
	result = dxInit_->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	ImGui_ImplDX12_Init(
		dxInit_->GetDevice(),
		static_cast<int>(dxInit_->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		srvHeap.Get(),
		srvHeap->GetCPUDescriptorHandleForHeapStart(),
		srvHeap->GetGPUDescriptorHandleForHeapStart()
	);

	ImGuiIO& io = ImGui::GetIO();

	//標準フォントを追加する
	io.Fonts->AddFontDefault();

}

void ImguiManager::Finalize()
{
	//後始末
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//デスクリプタヒープを解放
	srvHeap.Reset();

}

void ImguiManager::Begin()
{
	//Imguiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

}

void ImguiManager::End()
{
	//描画前準備
	ImGui::Render();
}

void ImguiManager::Draw()
{
	ID3D12GraphicsCommandList* commandList = dxInit_->GetCommandList();

	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);

}
