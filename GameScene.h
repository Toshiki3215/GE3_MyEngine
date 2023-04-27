#pragma once

#include "DirectXInitialize.h"
#include "Input.h"
#include <string>
#include "Object3d.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Model.h"
#include "Matrix4.h"
#include "Camera.h"
#include "ParticleManager.h"

/// <summary>
/// �Q�[���V�[��
/// </summary>
class GameScene
{
private: // �ÓI�����o�ϐ�
	//static const int debugTextTexNumber = 0;

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

	void Reset();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	void CamMove();
	void CamRota();
	void CamUpdate();

	void EffUpdate();
	void EffDraw();

	void EffUpdate2();
	void EffDraw2();


	Vector3 bVelocity(Vector3& velocity, Transform& worldTransform);
	
private: // �����o�ϐ� (�Œ�)
	DirectXInitialize* dxInit = nullptr;
	Input* input = nullptr;

	SpriteCommon* spriteCommon = nullptr;

	//�p�[�e�B�N���N���X�̏����� 
	ParticleManager* particleManager = nullptr;

private:	//�����o�ϐ�
	const float PI = 3.141592;

	//�J����
	Camera* camera = nullptr;
	Transform camWtf;
	Transform targetWtf;
	float targetTheta;
	float targetDistance = 10;
	float camMoveSpeed = 0.1f;
	float camRotaSpeed = PI/180;


	//�w�i�⏰
	Object3d* floor = nullptr;
	Model* floorMD = nullptr;
	/*Object3d* skydome = nullptr;
	Model* skydomeMD = nullptr;*/

	Object3d* obj = nullptr;
	Model* objMD = nullptr;

	Object3d* obj2 = nullptr;
	Model* obj2MD = nullptr;

	Sprite* tex1 = nullptr;
	Sprite* tex2 = nullptr;

	//�Q�[���t���[
	enum class Scene
	{
			Title,
			Select,
			Play,
			Clear,
			Gameover,
	};

	Scene scene;

	int stage;

	int isEffFlag;

};