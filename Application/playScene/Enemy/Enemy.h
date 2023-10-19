// Enemy.h
// Enemy���Ǘ����Ă���

#pragma once
#include "DirectXInitialize.h"
#include "Object3D.h"
#include "Input.h"
#include "Model.h"
#include "EnemyBullet.h"

class Enemy 
{
public:

	// ������
	void Initilize(Vector3 pos);

	// �X�V
	void Update(Vector3 pos);

	// �`��
	void Draw();

	//�����蔻��
	void OnCollision();

private:
	
	//���@
	Object3d* enemyObj = nullptr;
	Model* enemyMD = nullptr;

	Vector3 enemySpeed = { 0.0f,0.0f,0.05f };

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

	//�e����
	Object3d* shootObj_ = nullptr;
	Model* shootModel_ = nullptr;
	bool isShootFlag = false;

	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	float bulletSpeed = 0.0f;

	bool shotCool = false;

	static const int32_t shotCoolTime = 10;
	int32_t coolTimer = shotCoolTime;

	Vector3 len;

	bool shootFlg = true;

public:

	//�e�q�֌W
	void SetParent(Transform* parent) { enemyObj->wtf.parent_ = parent; }

	//�J�����Ƃ̐e�q�֌W
	void SetParentCamera(Vector3 cameraWtf);

	//�G�̒e���X�g
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

};
