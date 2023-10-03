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

class TitleScene
{
public:
	TitleScene();
	~TitleScene();

	void Initialize(DirectXInitialize* dxInit);
	void Update();
	void Draw();

private:
	SpriteCommon* spriteCommon = nullptr;
	Sprite* titleTex = nullptr;

	Object3d* titlePlayer = nullptr;
	Model* titlePlayerMD = nullptr;

};