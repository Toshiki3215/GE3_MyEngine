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

	/// <summary>
	/// 遷移演出が始まったか
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
	/// イーズイン関数
	/// </summary>
	/// <param name="time"></param>
	/// <param name="startPos"></param>
	/// <param name="endPos"></param>
	/// <param name="maxTime"></param>
	/// <returns></returns>
	float easeIn(float time, float startPos, float endPos, float maxTime);

	/// <summary>
	/// イーズアウト関数
	/// </summary>
	/// <param name="time"></param>
	/// <param name="startPos"></param>
	/// <param name="endPos"></param>
	/// <param name="maxTime"></param>
	/// <returns></returns>
	float easeOut(float time, float startPos, float endPos, float maxTime);

	/// <summary>
	/// 累乗(3乗)ーイージング用
	/// </summary>
	/// <param name="x"></param>
	/// <returns></returns>
	float pow3(float x);

	/// <summary>
	/// 累乗(5乗)ーイージング用
	/// </summary>
	/// <param name="x"></param>
	/// <returns></returns>
	float pow5(float x);

};