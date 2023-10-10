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

	void Initialize(DirectXInitialize* dxInit, Input* input);
	void UpdateStart();
	void UpdateEnd();
	void Draw();

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

	float sceneChangeSpead = 100.0f;

	float startTimer = 40.0f;

};