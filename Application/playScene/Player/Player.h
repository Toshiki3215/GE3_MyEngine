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
#include "Easing.h"

#include <memory>

class Player
{
public:
	Player();
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxCommon"></param>
	/// <param name="input"></param>
	void Initialize(DirectXInitialize* dxCommon, Input* input);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// FBXの描画
	/// </summary>
	void FbxDraw();

	//プレイヤーの行動一覧
	void PlayerAction();

	////ワールド座標を取得
	Vector3 GetWorldPosition();

	//ワールド座標を取得(弾)
	Vector3 GetBulletWorldPosition();

	//ワールド座標を取得(レティクル)
	Vector3 GetRetWorldPosition();

	/// <summary>
	/// ゲッター(ワールド座標)
	/// </summary>
	/// <returns></returns>
	Vector3 GetPos() { return playerObj->wtf.position; }

	/// <summary>
	/// セッター(ワールド座標)
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(Vector3 pos) { playerObj->wtf.position = pos; }

	/// <summary>
	/// 親子化(カメラ)
	/// </summary>
	/// <param name="cameraWtf"></param>
	void SetParentCamera(Matrix4 cameraWtf) { playerObj->wtf.matWorld = cameraWtf; }
	void SetParentCamera(Vector3 cameraWtf);

	//弾リストを取得
	//const std::list<PlayerBullet*>& GetBullets() { return bullets_; }
	std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }
	std::list<std::unique_ptr<PlayerBullet>>& GetBitBullets1() { return bitBullets1; }
	std::list<std::unique_ptr<PlayerBullet>>& GetBitBullets2() { return bitBullets2; }

	/// <summary>
	/// 親子化
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(Transform* parent) { playerObj->wtf.parent_ = parent; }

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision();

	bool GetAlive() { return isAlive; }

	void PlayerModeChange();

private:
	const float PI = 3.141592f;
	Input* input_ = nullptr;
	DirectXInitialize* dxInit = nullptr;
	Easing* easing = nullptr;

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
	Object3d* bulletObj_ = nullptr;
	Model* bulletModel_ = nullptr;
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

	float targetDistance = 10;
	float camMoveSpeed = 0.2f;

	Vector2 camRotaSpeed = { PI / 1800, PI / 1800 };

	Vector3 ret = { -1.5f,1.0f,10.0f };

	bool isAlive = true;

	bool modeChange = TRUE;

	//弾
	/*std::list<PlayerBullet*> bullets_;
	std::list<PlayerBullet*> bitBullets1;
	std::list<PlayerBullet*> bitBullets2;*/

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

	float playerHp = 5.0f;

	float bitSpace = 2.0f;

	float clearAnimeTimer = 0;
	float animeTimerEnd = 300;

	float startPos = 0;
	float endPos = 300;

	bool isBitDraw = TRUE;

	bool isRetDraw = TRUE;
	
};