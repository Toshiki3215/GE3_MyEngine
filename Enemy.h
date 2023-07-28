#pragma once
#include "DirectXInitialize.h"
#include "Object3D.h"
#include "Input.h"
#include "Model.h"

class Enemy 
{
public:

	// ������
	void Initilize();

	// �X�V
	void Update();

	// �`��
	void Draw();

	void OnCollision();

private:
	
	//���@
	Object3d* enemyObj = nullptr;
	Model* enemyMD = nullptr;

	Vector3 enemySpeed = { 0.0f,0.0f,1.0f };

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;

	bool isDead = false;

	enum class Phase 
	{
		Approch,
		Leave,
		Death,
	};

	//�t�F�[�Y
	Phase phase_ = Phase::Approch;
	//Phase phase_ = Phase::Death;

};
