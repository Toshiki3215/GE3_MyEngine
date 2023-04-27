#pragma once

#include "DirectXInitialize.h"
#include "Input.h"
#include <string>
#include "Object3d.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Model.h"
#include "Matrix4.h"
#include "Camera.h"
#include "ParticleManager.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene
{
private: // 静的メンバ変数
	//static const int debugTextTexNumber = 0;

public: // メンバ関数

	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXInitialize* dxInit, Input* input);

	void Reset();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void CamMove();
	void CamRota();
	void CamUpdate();

	void EffUpdate();
	void EffDraw();

	void EffUpdate2();
	void EffDraw2();


	Vector3 bVelocity(Vector3& velocity, Transform& worldTransform);
	
private: // メンバ変数 (固定)
	DirectXInitialize* dxInit = nullptr;
	Input* input = nullptr;

	SpriteCommon* spriteCommon = nullptr;

	//パーティクルクラスの初期化 
	ParticleManager* particleManager = nullptr;

private:	//メンバ変数
	const float PI = 3.141592;

	//カメラ
	Camera* camera = nullptr;
	Transform camWtf;
	Transform targetWtf;
	float targetTheta;
	float targetDistance = 10;
	float camMoveSpeed = 0.1f;
	float camRotaSpeed = PI/180;


	//背景や床
	Object3d* floor = nullptr;
	Model* floorMD = nullptr;
	/*Object3d* skydome = nullptr;
	Model* skydomeMD = nullptr;*/

	Object3d* obj = nullptr;
	Model* objMD = nullptr;

	Object3d* obj2 = nullptr;
	Model* obj2MD = nullptr;

	Sprite* tex1 = nullptr;
	Sprite* tex2 = nullptr;

	//ゲームフロー
	enum class Scene
	{
			Title,
			Select,
			Play,
			Clear,
			Gameover,
	};

	Scene scene;

	int stage;

	int isEffFlag;

};