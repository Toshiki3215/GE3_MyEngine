#pragma once
#include "DirectXInitialize.h"
#include "Object3D.h"
#include "Input.h"
#include "Model.h"

class Enemy 
{
public:

	// 初期化
	void Initilize();

	// 更新
	void Update();

	// 描画
	void Draw();

	void OnCollision();

private:
	
	//自機
	Object3d* enemyObj = nullptr;
	Model* enemyMD = nullptr;

	Vector3 enemySpeed = { 0.0f,0.0f,1.0f };

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	bool isDead = false;

	enum class Phase 
	{
		Approch,
		Leave,
		Death,
	};

	//フェーズ
	Phase phase_ = Phase::Approch;
	//Phase phase_ = Phase::Death;

};
