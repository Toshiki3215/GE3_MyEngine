// ImguiManager.h
// Imgui�̏����������Ȃǂ̊Ǘ�

#pragma once
#include "WindowsApp.h"
#include "DirectXInitialize.h"

class ImguiManager
{
public:

	void Initialize(WinApp* winApp, DirectXInitialize* dxInit);

	//�I��
	void Finalize();

	void Begin();

	void End();

	void Draw();

private:

	HRESULT result;

	DirectXInitialize* dxInit_;

	//SRV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;

};