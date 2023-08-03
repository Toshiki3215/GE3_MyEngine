#pragma once

#include "DirectXInitialize.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Object3d.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Model.h"
#include <string>
#include "ParticleManager.h"
#include "FBXObject.h"
#include "FbxLoader.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;


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

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXInitialize* dxInit = nullptr;
	//FBXで使うカメラ(ビュープロジェクション?みたいな)
	Camera* camera = nullptr;
	Input* input = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	//パーティクルクラスの初期化 
	//ParticleManager* particleManager = nullptr;

	FBXModel* fbxModel1 = nullptr;
	FBXObject* fbxObject1 = nullptr;

};