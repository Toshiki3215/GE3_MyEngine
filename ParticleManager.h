#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>

class ParticleManager
{
private:

	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// 定数バッファ用データ構造体（マテリアル）
	struct ConstBufferDataMaterial
	{
		XMFLOAT4 color; // 色 (RGBA)
	};

	// 頂点データ構造体
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz座標
		float scale;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMMATRIX mat;
		XMMATRIX matBillboard;	// ビルボード行列
	};

	//パーティクル一粒
	struct Particle 
	{
		using XMFLOAT3 = DirectX::XMFLOAT3;

		//座標
		XMFLOAT3 position = {};
		//速度
		XMFLOAT3 velocity = {};
		//加速度
		XMFLOAT3 accel = {};

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

		XMFLOAT4 color;
	};

private:
	static const int division = 50;	// 分割数
	static const float radius;		// 底面の半径
	static const float prizmHeight;	// 柱の高さ
	static const int planeCount = division * 2 + division * 2;// 面の数
	static const int vertexCount = 1024;

public:

	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	// 描画後処理
	static void PostDraw();

	// 3Dオブジェクト生成
	static ParticleManager* Create();

	// 視点座標の取得
	static const XMFLOAT3& GetEye() { return eye; }

	// 視点座標の設定
	static void SetEye(XMFLOAT3 eye);

	// 注視点座標の取得
	static const XMFLOAT3& GetTarget() { return target; }

	// 注視点座標の設定
	static void SetTarget(XMFLOAT3 target);

	// ベクトルによる視点移動
	static void CameraMoveEyeVector(XMFLOAT3 move);

private:

	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
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
	// ビュー行列
	static XMMATRIX matView;
	// 射影行列
	static XMMATRIX matProjection;
	// 視点座標
	static XMFLOAT3 eye;
	// 注視点座標
	static XMFLOAT3 target;
	// 上方向ベクトル
	static XMFLOAT3 up;
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// 頂点データ配列
	static VertexPos vertices[vertexCount];
	//ビルボード行列
	static XMMATRIX matBillboard;
	//Y軸回りビルボード行列
	static XMMATRIX matBillboardY;
	//パーティクル配列
	std::forward_list<Particle>particles;

private:

	// デスクリプタヒープの初期化
	static void InitializeDescriptorHeap();

	// カメラ初期化
	static void InitializeCamera(int window_width, int window_height);

	// グラフィックパイプライン生成
	static void InitializeGraphicsPipeline();

	// テクスチャ読み込み
	static void LoadTexture();

	// モデル作成
	static void CreateModel();

	// ビュー行列を更新
	static void UpdateViewMatrix();

public:

	bool Initialize();

	// 毎フレーム処理
	void Update();

	// 描画
	void Draw();

	// パーティクルの追加
	void Add(int life, XMFLOAT3 position, XMFLOAT3 velociy, XMFLOAT3 accel, float start_scale, float end_scale, XMFLOAT4 color);

private:

	ComPtr<ID3D12Resource> constBuff; // 定数バッファ

	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };

	ConstBufferDataMaterial* constMapMaterial = nullptr;
};
