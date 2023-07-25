#include "ImguiManager.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx12.h"
#include <cassert>
#include <vector>

void ImguiManager::Initialize(WinApp* winApp, DirectXInitialize* dxInit)
{
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
	result = dxInit->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	ImGui_ImplDX12_Init(
		dxInit->GetDevice(),
		static_cast<int>(dxInit->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		srvHeap.Get(),
		srvHeap->GetCPUDescriptorHandleForHeapStart(),
		srvHeap->GetGPUDescriptorHandleForHeapStart()
	);

	ImGuiIO& io = ImGui::GetIO();

	//標準フォントを追加する
	io.Fonts->AddFontDefault();

}
