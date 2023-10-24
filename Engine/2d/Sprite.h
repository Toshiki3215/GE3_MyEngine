// Sprite.h
// スプライトの読み込み、更新、描画処理

#pragma once
#include"SpriteCommon.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Affin.h"

// 頂点データ構造体
struct Vertex
{
	Vector3 pos; // xyz座標
	Vector2 uv;  // uv座標
};

class Sprite
{
public:
	//コンストラクタ
	Sprite();

	Sprite(uint32_t num, Vector2 pos, Vector2 size, Vector4 color, Vector2 anchorpoint, bool FlipX, bool FlipY);

	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// 定数バッファ用データ構造体（マテリアル）
	struct ConstBufferDataMaterial
	{
		Vector4 color; // 色 (RGBA)
	};

	//定数バッファ用構造体（３D変換行列）
	struct ConstBufferDataTransform
	{
		Matrix4 mat;	//3D変換行列
	};

	//頂点番号
	enum VertexNumber
	{
		LB,//左下
		LT,//左上
		RB,//右下
		RT,//右上
	};

public:
	//初期化
	void Initialize(SpriteCommon* spritecommon_, uint32_t textureIndex = UINT32_MAX);

	//更新
	void Update();

	//描画
	void Draw();

	//描画座標セット
	void SetPozition(const Vector2& position_);

	//ポジションの取得(ゲッター)
	const Vector2& GetPosition() const { return position; }

	//ポジションの変更(セッター)
	void SetRotation(float rotation_);

	//回転角
	float GetRotation() { return rotation; }

	//セッター(色)
	void SetColor(const Vector4& color_) { color = color_; }

	//ゲッター(色)
	Vector4 GetColor() { return color; }

	//セッター(スケール)
	void SetScale(const Vector3& scale_) { scale = scale_; }

	//ゲッター(スケール)
	Vector3 GetScale() { return scale; }

	//セッター(アンカーポイント)
	void SetAnchorPoint(const Vector2& anchorPoint_) { anchorPoint = anchorPoint_; }

	//ゲッター(アンカーポイント)
	Vector2 GetAnchorPoint() { return anchorPoint; }

	//セッター(テクスチャインデックス)
	void SetTextureIndex(uint32_t texNmb) { textureIndex_ = texNmb; AdjustTextureSize(); }

	//ゲッター(テクスチャインデックス)
	uint32_t GetTextureIndex() { return textureIndex_; }

	//セッター(テクスチャサイズ)
	void SetTexSize(Vector2 texSize) { textureSize = texSize; }

	//ゲッター(テクスチャサイズ)
	Vector2 GetTexSize() { return textureSize; }

	//セッター(サイズ)
	void SetSize(Vector2 size);

	/// 上下反転の設定
	void SetIsFlipY(bool isFlipY);

	/// 左右反転の設定
	void SetIsFlipX(bool isFlipX);

private:
	//テクスチャサイズをイメージに合わせる
	void AdjustTextureSize();

	SpriteCommon* spritecomon;
	HRESULT result;

	// 頂点データ
	Vertex vertices[4] =
	{
		// x      y     z       u     v
		{{-0.4f, -0.7f, 0.0f}, {0.0f, 1.0f}}, // 左下
		{{-0.4f, +0.7f, 0.0f}, {0.0f, 0.0f}}, // 左上
		{{+0.4f, -0.7f, 0.0f}, {1.0f, 1.0f}}, // 右下
		{{+0.4f, +0.7f, 0.0f}, {1.0f, 0.0f}}, // 右上
	};

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	Matrix4 matScale;//スケーリング行列
	Matrix4 matWorld;
	Matrix4 matRot;//回転行列
	Matrix4 matTrans;//平行移動行列

	//座標
	Vector3 scale{ 0.5f, 0.5f, 1.0f };

	Vector2 size_ = { 100.0f,100.0f };

	float rotation = 0.0f;
	Vector2 position = { 0.0f, 0.0f };

	Vector4 color = { 1,1,1,1 };

	Vector2 anchorPoint = { 0.0f,0.0f };

	ComPtr<ID3D12Resource> constBuffTransform;
	ConstBufferDataTransform* constMapTransform = nullptr;

	ConstBufferDataMaterial* constMapMaterial = nullptr;

	ComPtr<ID3D12Resource> constBuffMaterial;

	Vertex vertices_[4];

	Vertex* vertMap = nullptr;

	//テクスチャ番号
	uint32_t textureIndex_ = 0;

	//テクスチャ左上座標
	Vector2 textureLeftTop = { 0.0f,0.0f };

	//テクスチャ切り出しサイズ
	Vector2 textureSize = { 100.0f,100.0f };

	Matrix4 matProjection;

	// アンカーポイント
	Vector2 anchorpoint = { 0, 0 };

	// 頂点バッファの生成
	ComPtr<ID3D12Resource> vertBuff = nullptr;

	// 左右反転
	bool isFlipX = false;

	// 上下反転
	bool isFlipY = false;
};

