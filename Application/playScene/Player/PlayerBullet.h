// PlayerBullet.h
// プレイヤーの攻撃(弾)を管理している

#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Input.h"

class PlayerBullet
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initilize(Object3d* playerObj, Object3d* retObj, Vector3 scale);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(Vector3 enemylen, Vector3 len, float shootSpeed, Object3d* playerObj, Object3d* retObj);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw();

	/// <summary>
	/// 弾の生存
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; }

	/// <summary>
	/// ワールド座標取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPos();

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision();

private:
	
	//モデル
	Object3d* bulletObj = nullptr;
	Model* bulletModel = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 1;
	//static const int32_t kLifeTime = 10 * 1;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスフラグ
	bool isDead_ = false;

	Vector3 target;

};