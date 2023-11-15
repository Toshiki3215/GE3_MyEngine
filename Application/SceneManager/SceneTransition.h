// SceneTransition.h
// シーン遷移の管理

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
#include "Vector4.h"
#include "Easing.h"

class SceneTransition
{
public:
	SceneTransition();

	~SceneTransition();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxInit"></param>
	/// <param name="input"></param>
	void Initialize(DirectXInitialize* dxInit, Input* input);

	/// <summary>
	/// 更新処理(演出のはじめ部分)
	/// </summary>
	void UpdateStart();

	/// <summary>
	/// 更新処理(演出の終わり部分)
	/// </summary>
	void UpdateEnd();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void UpdateColor(Vector4 color);
	void Draw2();

	void endDraw();

	/// <summary>
	/// 遷移演出が始まったか
	/// </summary>
	/// <returns></returns>
	bool GetTransF() { return  isTrans; }

	void EndText();

	void Reset();

private:
	Input* input_ = nullptr;
	Easing* easing_ = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Sprite* transTex1 = nullptr;
	Sprite* transTex2 = nullptr;
	Sprite* UITex = nullptr;
	Sprite* endTex = nullptr;

	float leftTransPos = -1000.0f;
	float rightTransPos = 1000.0f;
	float defaultPos = 0.0f;
	float defaultPos2 = 1000.0f;

	bool isTrans = TRUE;

	float startTimer = 60.0f;
	float animeTimer = 0.0f;
	float maxTime = 1200.0f;

	float animeTimer2 = 0;
	float pos1 = -720;
	float pos2 = 0;
	float pos3 = -720;

	float a = 0.5f;

	Vector4 color = { 1,1,1,a };

};