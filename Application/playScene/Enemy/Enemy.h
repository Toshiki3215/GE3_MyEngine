// Enemy.h
// Enemyを管理している

#pragma once
#include "DirectXInitialize.h"
#include "Object3D.h"
#include "Input.h"
#include "Model.h"
#include "EnemyBullet.h"

class Enemy 
{
public:

	// 初期化
	void Initilize(Vector3 pos);

	// 更新
	void Update(Vector3 pos);

	// 描画
	void Draw();

	//当たり判定
	void OnCollision();

private:
	
	//自機
	Object3d* enemyObj = nullptr;
	Model* enemyMD = nullptr;

	Vector3 enemySpeed = { 0.0f,0.0f,0.05f };

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

	//弾発射
	Object3d* shootObj_ = nullptr;
	Model* shootModel_ = nullptr;
	bool isShootFlag = false;

	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	float bulletSpeed = 0.0f;

	bool shotCool = false;

	static const int32_t shotCoolTime = 10;
	int32_t coolTimer = shotCoolTime;

	Vector3 len;

	bool shootFlg = true;

public:

	//親子関係
	void SetParent(Transform* parent) { enemyObj->wtf.parent_ = parent; }

	//カメラとの親子関係
	void SetParentCamera(Vector3 cameraWtf);

	//敵の弾リスト
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

};
