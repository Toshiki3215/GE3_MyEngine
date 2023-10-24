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

	/// <summary>
	/// �������p���Z�b�g�֐�
	/// </summary>
	void Reset();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �f�o�b�O�J����
	/// </summary>
	void CameraMove();

	/// <summary>
	/// �J�����̍X�V����
	/// </summary>
	void CameraUpdate();

	/// <summary>
	/// �X�^�[�g���o
	/// </summary>
	void GameStartEfe(bool isStart, bool isStart2);

	/// <summary>
	/// �p�[�e�B�N���X�V����
	/// </summary>
	void EffUpdate();
	void EffUpdate2();

	/// <summary>
	/// �p�[�e�B�N���`��
	/// </summary>
	void EffDraw();
	void EffDraw2();

	/// <summary>
	/// �����蔻��
	/// </summary>
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
	float targetTheta = 0.0f;
	float targetDistance = 10;
	float camMoveSpeed = 0.1f;
	float camRotaSpeed = PI/180;

	RailCamera* railCamera = nullptr;

	//���f��
	Object3d* skydome = nullptr;
	Model* skydomeMD = nullptr;

	Object3d* targetObj = nullptr;
	Model* targetMD = nullptr;

	//�Q�[���t���[
	enum class Scene
	{
			Title,
			Select,
			Play,
			Clear,
			Gameover,
	};

	Scene scene = Scene::Title;

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

	Vector3 cameraMoveSpeed = { 0,0,0 };

	float startT = 0;

	bool isStart = FALSE;
	bool isStart2 = FALSE;

	Vector4 a = { 0,0,0,0.1f};
	Vector4 color = { 1,1,1,1 };

};