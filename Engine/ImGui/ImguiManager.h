// ImguiManager.h
// Imgui�̏����������Ȃǂ̊Ǘ�

#pragma once
#include "WindowsApp.h"
#include "DirectXInitialize.h"

class ImguiManager
{
public:

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="winApp"></param>
	/// <param name="dxInit"></param>
	void Initialize(WinApp* winApp, DirectXInitialize* dxInit);

	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();

	/// <summary>
	/// Imgui�t���[���J�n
	/// </summary>
	void Begin();

	/// <summary>
	/// �`��O����
	/// </summary>
	void End();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:

	HRESULT result;

	DirectXInitialize* dxInit_;

	//SRV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;

};