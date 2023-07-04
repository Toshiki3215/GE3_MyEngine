#pragma once
#include "Matrix4.h"
#include "input.h"
#include "Model.h"
#include "Object3D.h"
#include "PlayerBullet.h"

class Player
{
public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Input* input);

	//void Reset();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	Vector3 GetPlayerPos();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	//void OnCollision();

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

private:
	Input* input = nullptr;

	Vector3 playerPos;

	Vector3 playerRot;

	Vector3 playerScale;

	Vector3 moveTarget;
	Vector3 rotaTarget;

	Object3d* playerObj = nullptr;
	Model* playerModel = nullptr;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

};