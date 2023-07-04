#pragma once
#include "Matrix4.h"
#include "input.h"
#include "Model.h"
#include "Object3D.h"
#include "PlayerBullet.h"

class Player
{
public: // ƒƒ“ƒoŠÖ”

	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void Initialize(Input* input);

	//void Reset();

	/// <summary>
	/// –ˆƒtƒŒ[ƒ€ˆ—
	/// </summary>
	void Update();

	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw();

	Vector3 GetPlayerPos();

	/// <summary>
	/// UŒ‚
	/// </summary>
	void Attack();

	//void OnCollision();

	//’eƒŠƒXƒg‚ğæ“¾
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

	//’e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

};