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
	void Initilize(const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw();

	bool IsDead() const { return isDead_; }

	//Vector3 BulletPos() const { return worldTransformBullet_.translation_; }

	Vector3 GetWorldPos();

	//void OnCollision();

private:
	
	Vector3 bulletPos;
	Vector3 bulletRot;
	Vector3 bulletScale;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//モデル
	Object3d* bulletObj = nullptr;
	Model* bulletModel = nullptr;

	//速度
	Vector3 velocity_;

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスフラグ
	bool isDead_ = false;

};