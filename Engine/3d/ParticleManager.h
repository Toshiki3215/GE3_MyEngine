// ParticleManager.h
// パーティクルの管理をしている

#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <forward_list>
#include "Camera.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Affin.h"

class ParticleManager
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: 
	// 定数バッファ用データ構造体（マテリアル）
	struct ConstBufferDataMaterial 
	{
		Vector4 color; // 色 (RGBA)
	};

	// 頂点データ構造体
	struct VertexPos
	{
		Vector3 pos; // xyz座標
		float scale;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		Matrix4 mat;
		Matrix4 matBillboard;	// ビルボード行列
	};

	//パーティクル一粒
	struct Particle 
	{

		//座標
		Vector3 position = {};

		//速度
		Vector3 velocity = {};

		//加速度
		Vector3 accel = {};

		//現在フレーム
		int frame = 0;

		//過去フレーム
		int num_frame = 0;

		//スケール
		float scale = 1.0f;

		//初期値
		float s_scale = 1.0f;

		//最終値
		float e_scale = 0.0f;

		Vector4 color;
	};

private:

	static const int division = 50;		    // 分割数
	static const float radius;				// 底面の半径
	static const float prizmHeight;			// 柱の高さ
	static const int planeCount = division * 2 + division * 2; // 面の数
	static const int vertexCount = 1024;

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
	/// パーティクル作成
	/// </summary>
	/// <returns></returns>
	static ParticleManager* Create();



private:

	// デバイス
	static ComPtr <ID3D12Device> device;

	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;

	// コマンドリスト
	static ComPtr <ID3D12GraphicsCommandList> cmdList;

	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;

	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;

	// 頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;

	// テクスチャバッファ
	static ComPtr<ID3D12Resource> texbuff;

	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;

	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;

	// 頂点データ配列
	static VertexPos vertices[vertexCount];

	//ビルボード行列
	static Matrix4 matBillboard;

	//Y軸回りビルボード行列
	static Matrix4 matBillboardY;

	//パーティクル配列
	std::forward_list<Particle>particles;

private:

	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	static void InitializeDescriptorHeap();

	/// <summary>
	/// グラフィックスパイプラインの初期化
	/// </summary>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// モデル生成
	/// </summary>
	static void CreateModel();


public:

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="fileName"></param>
	static void LoadTexture(const std::string& fileName);

	ParticleManager();
	~ParticleManager();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize();
	
	// 毎フレーム処理
	void Update();

	// 描画
	void Draw();

	// パーティクルの追加
	//生存時間,初期座標,速度,加速度
	void Add(int life, Vector3 position, Vector3 velociy, Vector3 accel, float start_scale, float end_scale);

	//セッター(カメラ)
	static void SetCamera(Camera* camera) { ParticleManager::camera = camera; }

private:

	ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	static Camera* camera;

	// ローカルスケール
	Vector3 scale = { 1,1,1 };

	ConstBufferDataMaterial* constMapMaterial = nullptr;
};