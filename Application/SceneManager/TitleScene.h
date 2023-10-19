// TitleScene.h
// タイトルシーンを管理している

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

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxInit"></param>
	/// <param name="input"></param>
	void Initialize(DirectXInitialize* dxInit, Input* input);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// タイトルシーンからゲームシーンに移動するか
	/// </summary>
	/// <returns></returns>
	bool GetStartF() { return  startFlg; }

private:
	Input* input_ = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Sprite* titleTex = nullptr;
	Sprite* selectBar = nullptr;
	Sprite* selectBar2 = nullptr;
	Sprite* selectBar3 = nullptr;
	Sprite* text1 = nullptr;
	Sprite* text2 = nullptr;

	float selectBarPosX = -400.0f;
	float selectBarPosY = 459.0f;

	float posMax = 459.0f;
	float posMin = 588.0f;

	bool startFlg = FALSE;

	int animeTimer = 0;

};