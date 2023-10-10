#include "ImguiManager.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include <cassert>
#include <vector>

void ImguiManager::Initialize(WinApp* winApp, DirectXInitialize* dxInit)
{
	dxInit_ = dxInit;

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

	//�W���t�H���g��ǉ�����
	io.Fonts->AddFontDefault();

}

void ImguiManager::Finalize()
{
	//��n��
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//�f�X�N���v�^�q�[�v�����
	srvHeap.Reset();

}

void ImguiManager::Begin()
{
	//Imgui�t���[���J�n
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

}

void ImguiManager::End()
{
	//�`��O����
	ImGui::Render();
}

void ImguiManager::Draw()
{
	ID3D12GraphicsCommandList* commandList = dxInit_->GetCommandList();

	//�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = {srvHeap.Get()};
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// �`��R�}���h�𔭍s
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);

}
