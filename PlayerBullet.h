#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Input.h"

class PlayerBullet
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	void Initilize(const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw();

	bool IsDead() const { return isDead_; }

	//Vector3 BulletPos() const { return worldTransformBullet_.translation_; }

	Vector3 GetWorldPos();

	//void OnCollision();

private:
	
	Vector3 bulletPos;
	Vector3 bulletRot;
	Vector3 bulletScale;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;

	//���f��
	Object3d* bulletObj = nullptr;
	Model* bulletModel = nullptr;

	//���x
	Vector3 velocity_;

	//����<frm>
	static const int32_t kLifeTime = 60 * 5;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;

	//�f�X�t���O
	bool isDead_ = false;

};