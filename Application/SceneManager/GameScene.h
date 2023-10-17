// GameScene.h
// �Q�[���V�[�����Ǘ����Ă���

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
#include "RailCamera.h"
#include "ParticleManager.h"
#include "FBXObject.h"
#include "Player.h"
#include "Enemy.h"
#include "TitleScene.h"
#include "SceneTransition.h"

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
	void CamMove2();
	void CamRota();
	void CamUpdate();
	void CamUpdate2();

	void CameraMove();
	void CameraUpdate();

	void EffUpdate();
	void EffDraw();

	void EffUpdate2();
	void EffDraw2();

	Vector3 bVelocity(Vector3& velocity, Transform& worldTransform);

	void CheckAllCollisions();
	
private: // �����o�ϐ� (�Œ�)
	DirectXInitialize* dxInit = nullptr;
	Input* input = nullptr;

	SpriteCommon* spriteCommon = nullptr;

	//�p�[�e�B�N���N���X�̏����� 
	ParticleManager* particleManager = nullptr;

private:	//�����o�ϐ�
	const float PI = 3.141592f;

	//�J����
	Camera* camera = nullptr;
	Transform camWtf;
	Transform targetWtf;
	float targetTheta;
	float targetDistance = 10;
	float camMoveSpeed = 0.1f;
	float camRotaSpeed = PI/180;

	RailCamera* railCamera = nullptr;

	//���f��
	Object3d* skydome = nullptr;
	Model* skydomeMD = nullptr;

	Object3d* playerObj = nullptr;
	Model* playerMD = nullptr;

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

	int isEffFlag = 0;

	FBXModel* fbxModel1 = nullptr;

	FBXObject* fbxObject1 = nullptr;
	
	FBXModel* fbxModel2 = nullptr;

	FBXObject* fbxObject2 = nullptr;

	Vector3 playerPos;

	Player* player_ = nullptr;

	Enemy* enemy_ = nullptr;
	Enemy* enemy2_ = nullptr;

	bool railCameraSwitch = false;

	TitleScene* titleScene = nullptr;

	SceneTransition* sceneTrans = nullptr;

	float rSpeed = 0.0f;

};