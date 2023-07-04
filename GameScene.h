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
#include "FBXObject.h"
#include "Player.h"

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
	void CamMove2(Vector3 pos);
	void CamRota();
	void CamUpdate();
	void CamUpdate2(Vector3 pos);

	void PlayerUpdata(Vector3 pos);

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
	const float PI = 3.141592f;

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
	Object3d* skydome = nullptr;
	Model* skydomeMD = nullptr;

	Object3d* obj = nullptr;
	Model* objMD = nullptr;

	Object3d* obj2 = nullptr;
	Model* obj2MD = nullptr;

	Object3d* playerObj = nullptr;
	Model* playerMD = nullptr;

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

	FBXModel* fbxModel1 = nullptr;

	FBXObject* fbxObject1 = nullptr;
	
	FBXModel* fbxModel2 = nullptr;

	FBXObject* fbxObject2 = nullptr;

	/*FBXModel* playerModel = nullptr;
	FBXObject* playerFbx = nullptr;*/

	Vector3 playerPos;

	Player* player_ = nullptr;

};