#pragma once

#include "FBXModel.h"
#include "FBXLoader.h"
#include "Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

class FBXObject
{
protected:

	//Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//DirectX::���ȗ�
	/*using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;*/

public:

	//setter
	static void SetDevice(ID3D12Device* device) { FBXObject::device = device; }
	static void SetCamera(Camera* camera) { FBXObject::camera = camera; }

private:

	//�f�o�C�X
	static ID3D12Device* device;

	//�J����
	static Camera* camera;

public:

	//�T�u�N���X

	//�萔�o�b�t�@�p�f�[�^�\����(���W�ϊ��s��p)
	struct ConstBufferDataTransform
	{
		Matrix4 viewproj;	//�r���[�v���W�F�N�V�����s��
		Matrix4 world;		//���[���h���W
		Vector3 cameraPos;	//�J�������W(���[���h���W)
	};

public:
	//�����o�֐�

	//������
	void Initialize();

	//�O���t�B�b�N�p�C�v���C���̐���
	static void CreateGraphicsPipeline();

protected:
	//�����o�ϐ�

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTrans;

	//���[�J���X�P�[��
	Vector3 scale = { 1,1,1 };

	//X,Y,Z�����̃��[�J����]�p
	Vector3 rotation = { 0,0,0 };

	//���[�J�����W
	Vector3 position = { 0,0,0 };

	//���[�J�����[���h�ϊ��s��
	Matrix4 matWorld;

	//�萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBuffSkin;


private:

	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;

	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;

	CD3DX12_HEAP_PROPERTIES cbheapprop{};

	CD3DX12_RESOURCE_DESC cbresdesc{};
	CD3DX12_RESOURCE_DESC cbresdesc2{};

	//���f��
	FBXModel* fbxModel = nullptr;
 
public:

	//���t���[���X�V
	void Update();

	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//���f���̃Z�b�g
	void SetModel(FBXModel* fbxModel) { this->fbxModel = fbxModel; }

	//�萔
	//�{�[���̍ő吔
	static const int MAX_BONES = 32;

	//�萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin
	{
		Matrix4 bones[MAX_BONES];
	};

};