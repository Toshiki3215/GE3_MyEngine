#pragma once

#include "DirectXInitialize.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Object3d.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Model.h"
#include <string>
#include "ParticleManager.h"
#include "FBXObject.h"
#include "FbxLoader.h"

/// <summary>
/// �Q�[���V�[��
/// </summary>
class GameScene
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;


public: // �����o�֐�

	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	GameScene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameScene();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(DirectXInitialize* dxInit, Input* input);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private: // �����o�ϐ�
	DirectXInitialize* dxInit = nullptr;
	//FBX�Ŏg���J����(�r���[�v���W�F�N�V����?�݂�����)
	Camera* camera = nullptr;
	Input* input = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	

	/// <summary>
	/// �Q�[���V�[���p
	/// </summary>

	//�p�[�e�B�N���N���X�̏����� 
	//ParticleManager* particleManager = nullptr;

	FBXModel* fbxModel1 = nullptr;
	FBXObject* fbxObject1 = nullptr;

};