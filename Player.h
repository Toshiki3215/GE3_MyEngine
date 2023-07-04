#pragma once
#include "Matrix4.h"
#include "input.h"
#include "Model.h"
#include "Object3D.h"
#include "PlayerBullet.h"

class Player
{
public: // �����o�֐�

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Input* input);

	//void Reset();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	Vector3 GetPlayerPos();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	//void OnCollision();

	//�e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

private:
	Input* input = nullptr;

	Vector3 playerPos;

	Vector3 playerRot;

	Vector3 playerScale;

	Vector3 moveTarget;
	Vector3 rotaTarget;

	Object3d* playerObj = nullptr;
	Model* playerModel = nullptr;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

};