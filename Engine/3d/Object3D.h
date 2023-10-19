// Object3D.h
// モデルの初期化、更新処理、描画などの管理

#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string.h>
#include "Model.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Affin.h"
#include "Transform.h"
#include "Camera.h"

class Object3d
{
private:

	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// 定数バッファ用データ構造体
	struct ConstBufferDataB0
	{
		Matrix4 mat;	// ３Ｄ変換行列
	};

private:

	static const int division = 50;			// 分割数
	static const float radius;				// 底面の半径
	static const float prizmHeight;			// 柱の高さ
	static const int planeCount = division * 2 + division * 2; // 面の数
	static const int vertexCount = planeCount * 3;		       // 頂点数

public: 

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device"></param>
	/// <param name="window_width"></param>
	/// <param name="window_height"></param>
	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList"></param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <returns></returns>
	static Object3d* Create();

	/// <summary>
	/// 生存しているか
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return  isDead_; }

private:

	// デバイス
	static ComPtr<ID3D12Device> device;

	// コマンドリスト
	static ComPtr<ID3D12GraphicsCommandList> cmdList;

	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;

	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

	// ビュー行列
	static Matrix4 matView;
	// 射影行列
	static Matrix4 matProjection;
	// 視点座標
	static Vector3 eye;
	// 注視点座標
	static Vector3 target;
	// 上方向ベクトル
	static Vector3 up;

	static float focalLengs;

private:

	static void InitializeCamera(int window_width, int window_height);

	static void InitializeGraphicsPipeline();

	// ビュー行列を更新
	static void UpdateViewMatrix();

public:

	Object3d();
	~Object3d();

	//初期化
	bool Initialize();

	// 毎フレーム処理
	void Update();
	void Update(Transform* parentWtf);

	//行列の更新
	void UpdateMat();

	// 描画
	void Draw();

	/// <summary>
	/// ゲッター(親子関係)
	/// </summary>
	/// <returns></returns>
	Object3d* GetParent() const { return parent; }

	/// <summary>
	/// セッター(親子関係)
	/// </summary>
	/// <param name="parent_"></param>
	void SetParent(Object3d* parent_) { this->parent = parent_; }

	/// <summary>
	/// セッター(カメラ)
	/// </summary>
	/// <param name="camera_"></param>
	static void SetCamera(Camera* camera_) { Object3d::camera = camera_; }

	/// <summary>
	/// セッター(モデル)
	/// </summary>
	/// <param name="model_"></param>
	void SetModel(Model* model_) { this->model = model_; }

private: 
	// メンバ変数

public:

	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ

	bool isDead_ = false;

	// 色
	Vector4 color = { 1,1,1,1 };

	// 親オブジェクト
	Object3d* parent = nullptr;

	//モデル
	Model* model = nullptr;
	static Camera* camera;

	static float win_wi, win_hi;

public:

	Transform wtf;

};