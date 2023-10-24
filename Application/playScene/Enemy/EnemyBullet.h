// EnemyBullet.h
// Enemyの攻撃(弾)を管理している

#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Input.h"

class EnemyBullet
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initilize(Object3d* enemyObj);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(Vector3 len, float shootSpeed, Object3d* enemyObj);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw();

	//敵が生きているか
	bool IsDead() const { return isDead_; }

	//ワールド座標取得
	Vector3 GetWorldPos();

	//当たり判定
	void OnCollision();

private:

	//モデル
	Object3d* bulletObj = nullptr;
	Model* bulletModel = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 1;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスフラグ
	bool isDead_ = false;

	Vector3 target;

};