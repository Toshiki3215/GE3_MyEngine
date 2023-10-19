// PlayerBullet.h
// �v���C���[�̍U��(�e)���Ǘ����Ă���

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
	void Initilize(Object3d* playerObj, Object3d* retObj, Vector3 scale);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(Vector3 enemylen, Vector3 len, float shootSpeed, Object3d* playerObj, Object3d* retObj);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw();

	/// <summary>
	/// �e�̐���
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; }

	/// <summary>
	/// ���[���h���W�擾
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPos();

	/// <summary>
	/// �����蔻��
	/// </summary>
	void OnCollision();

private:
	
	//���f��
	Object3d* bulletObj = nullptr;
	Model* bulletModel = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;

	//����<frm>
	static const int32_t kLifeTime = 60 * 1;
	//static const int32_t kLifeTime = 10 * 1;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;

	//�f�X�t���O
	bool isDead_ = false;

	Vector3 target;

};