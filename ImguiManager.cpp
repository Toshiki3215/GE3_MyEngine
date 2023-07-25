#include "ImguiManager.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx12.h"
#include <cassert>
#include <vector>

void ImguiManager::Initialize(WinApp* winApp, DirectXInitialize* dxInit)
{
	//Imgui�̃R���e�L�X�g�𐶐�
	ImGui::CreateContext();

	//Imgui�̃X�^�C����ݒ�
	ImGui::StyleColorsLight();

	ImGui_ImplWin32_Init(winApp->GetHwnd());

	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	//�f�X�N���v�^�q�[�v����
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

	//�W���t�H���g��ǉ�����
	io.Fonts->AddFontDefault();

}
