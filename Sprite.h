#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class Sprite
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
	// 頂点データ構造体
	struct VertexPosUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT2 uv;  // uv座標
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX mat;	// ３Ｄ変換行列
	};

public:

	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	static void LoadTexture(UINT texnumber, const wchar_t* filename);

	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	static void PostDraw();

	static Sprite* Create(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color = { 1, 1, 1, 1 }, XMFLOAT2 anchorpoint = { 0.0f, 0.0f }, bool isFlipX = false, bool isFlipY = false);

	// 座標の取得
	const XMFLOAT2& GetPosition() { return position; }

	//カラー替え
	void SetColor(const XMFLOAT4& color) { this->color = color; }



private: 
	// 静的メンバ変数
	
	// テクスチャの最大枚数
	static const int srvCount = 512;

	// 頂点数
	static const int vertNum = 4;

	// デバイス
	static ID3D12Device* device;

	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;

	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;

	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature;

	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelineState;

	// 射影行列
	static XMMATRIX matProjection;

	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;

	// テクスチャバッファ
	static ComPtr<ID3D12Resource> texBuff[srvCount];

public:
	// メンバ関数
	
	// コンストラクタ
	Sprite(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);

	// 初期化
	bool Initialize();

	void SetRotation(float rotation);

	// 座標の設定
	void SetPosition(const XMFLOAT2& position);

	// サイズの設定
	void SetSize(const XMFLOAT2& size);

	// アンカーポイントの設定
	void SetAnchorPoint(const XMFLOAT2& anchorpoint);

	// 左右反転の設定
	void SetIsFlipX(bool isFlipX);

	// 上下反転の設定
	void SetIsFlipY(bool isFlipY);

	// テクスチャ範囲設定
	void SetTextureRect(const XMFLOAT2& texBase, const XMFLOAT2& texSize);

	// 描画
	void Draw();

private: 
	// メンバ変数

	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;

	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// テクスチャ番号
	UINT texNumber = 0;

	// Z軸回りの回転角
	float rotation = 0.0f;

	// 座標
	XMFLOAT2 position{};

	// スプライト幅、高さ
	XMFLOAT2 size = { 100.0f, 100.0f };

	// アンカーポイント
	XMFLOAT2 anchorpoint = { 0, 0 };

	// ワールド行列
	XMMATRIX matWorld{};

	// 色
	XMFLOAT4 color = { 1, 1, 1, 1 };

	// 左右反転
	bool isFlipX = false;

	// 上下反転
	bool isFlipY = false;

	// テクスチャ始点
	XMFLOAT2 texBase = { 0, 0 };

	// テクスチャ幅、高さ
	XMFLOAT2 texSize = { 100.0f, 100.0f };

private:
	// メンバ関数
	// 頂点データ転送
	void TransferVertices();

};

