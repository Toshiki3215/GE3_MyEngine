// Player.h
// Playerの処理を管理している

#pragma once

#include "DirectXInitialize.h"
#include "Object3d.h"
#include "Input.h"
#include "Camera.h"
#include "PlayerBullet.h"

#include "ParticleManager.h"

#include "Object3D.h"
#include "Model.h"

class Player
{
public:
	Player();
	~Player();

	void Initialize(DirectXInitialize* dxCommon, Input* input);
	void Update();

	void Draw();
	void FbxDraw();

	//プレイヤーの行動一覧
	void PlayerAction();

	Vector3 bVelocity(Vector3& velocity, Transform& worldTransform);

	////ワールド座標を取得
	Vector3 GetWorldPosition();

	//ワールド座標を取得(弾)
	Vector3 GetBulletWorldPosition();

	//ワールド座標を取得(レティクル)
	Vector3 GetRetWorldPosition();

	Vector3 GetPos() { return playerObj->wtf.position; }

	// ポジション
	void SetPos(Vector3 pos) { playerObj->wtf.position = pos; }

	void SetParentCamera(Matrix4 cameraWtf) { playerObj->wtf.matWorld = cameraWtf; }
	void SetParentCamera(Vector3 cameraWtf);

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	void SetParent(Transform* parent) { playerObj->wtf.parent_ = parent; }
	
	void OnCollision();

private:
	const float PI = 3.141592f;
	Input* input_ = nullptr;
	DirectXInitialize* dxInit = nullptr;
	
	//自機
	Object3d* playerObj = nullptr;
	Model* playerMD = nullptr;
	Model* playerMD2 = nullptr;

	//ビット
	Object3d* bitObj1 = nullptr;
	Model* bitModel1 = nullptr;

	Object3d* bitObj2 = nullptr;
	Model* bitModel2 = nullptr;

	//弾発射
	Object3d* shootObj_ = nullptr;
	Model* shootModel_ = nullptr;
	bool isShootFlag = false;

	//レティクル
	Object3d* retObj_ = nullptr;
	Model* retModel_ = nullptr;
	Vector3 enemylen;
	Vector3 len;

	const float moveSpeed_ = 0.1f;
	const float rotaSpeed_ = 0.1f;

	Camera* camera = nullptr;
	Transform* camTransForm = nullptr;
	Vector3 targetPos;
	Vector3 eyePos;
	Vector3 centerPos;
	//float targetTheta;
	float targetDistance = 10;
	float camMoveSpeed = 0.2f;

	Vector2 camRotaSpeed = { PI / 1800, PI / 1800 };

	Vector3 ret = { -1.5f,1.0f,10.0f };

	bool isAliveFlag = true;

	bool modeChange = false;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	std::list<std::unique_ptr<PlayerBullet>> bitBullets1;
	std::list<std::unique_ptr<PlayerBullet>> bitBullets2;

	float bulletSpeed = 0.05f;

	bool shotCool = false;

	Vector3 bulletScale = { 0.1f,0.1f,0.1f };

	static const int32_t shotCoolTime = 0;
	int32_t coolTimer = shotCoolTime;

	float rotaTime = 0.0f;
	float bitRotaTime = 20.0f;

	float rSpeed = 0.0f;

};