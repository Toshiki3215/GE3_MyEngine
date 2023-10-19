// SceneTransition.h
// �V�[���J�ڂ̊Ǘ�

#pragma once

#include "DirectXInitialize.h"
#include "Object3d.h"
#include "Input.h"
#include "Camera.h"
#include "PlayerBullet.h"
#include "ParticleManager.h"
#include "Object3D.h"
#include "Model.h"
#include "SpriteCommon.h"
#include "Sprite.h"

class SceneTransition
{
public:
	SceneTransition();

	~SceneTransition();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="dxInit"></param>
	/// <param name="input"></param>
	void Initialize(DirectXInitialize* dxInit, Input* input);

	/// <summary>
	/// �X�V����(���o�̂͂��ߕ���)
	/// </summary>
	void UpdateStart();

	/// <summary>
	/// �X�V����(���o�̏I��蕔��)
	/// </summary>
	void UpdateEnd();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �J�ډ��o���n�܂�����
	/// </summary>
	/// <returns></returns>
	bool GetTransF() { return  isTrans; }

private:
	Input* input_ = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Sprite* transTex1 = nullptr;
	Sprite* transTex2 = nullptr;

	float leftTransPos = -1000.0f;
	float rightTransPos = 1000.0f;
	float defaultPos = 0.0f;
	float defaultPos2 = 1000.0f;

	bool isTrans = TRUE;

	float startTimer = 60.0f;
	float animeTimer = 0.0f;
	float maxTime = 1200.0f;

public:
	
	/// <summary>
	/// �C�[�Y�C���֐�
	/// </summary>
	/// <param name="time"></param>
	/// <param name="startPos"></param>
	/// <param name="endPos"></param>
	/// <param name="maxTime"></param>
	/// <returns></returns>
	float easeIn(float time, float startPos, float endPos, float maxTime);

	/// <summary>
	/// �C�[�Y�A�E�g�֐�
	/// </summary>
	/// <param name="time"></param>
	/// <param name="startPos"></param>
	/// <param name="endPos"></param>
	/// <param name="maxTime"></param>
	/// <returns></returns>
	float easeOut(float time, float startPos, float endPos, float maxTime);

	/// <summary>
	/// �ݏ�(3��)�[�C�[�W���O�p
	/// </summary>
	/// <param name="x"></param>
	/// <returns></returns>
	float pow3(float x);

	/// <summary>
	/// �ݏ�(5��)�[�C�[�W���O�p
	/// </summary>
	/// <param name="x"></param>
	/// <returns></returns>
	float pow5(float x);

};