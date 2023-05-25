#pragma once

#include "FBXModel.h"
#include "FBXLoader.h"
#include "Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

class FBXObject
{
protected:

	//Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//DirectX::を省略
	/*using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;*/

public:

	//setter
	static void SetDevice(ID3D12Device* device) { FBXObject::device = device; }
	static void SetCamera(Camera* camera) { FBXObject::camera = camera; }

private:

	//デバイス
	static ID3D12Device* device;

	//カメラ
	static Camera* camera;

public:

	//サブクラス

	//定数バッファ用データ構造体(座標変換行列用)
	struct ConstBufferDataTransform
	{
		Matrix4 viewproj;	//ビュープロジェクション行列
		Matrix4 world;		//ワールド座標
		Vector3 cameraPos;	//カメラ座標(ワールド座標)
	};

public:
	//メンバ関数

	//初期化
	void Initialize();

	//グラフィックパイプラインの生成
	static void CreateGraphicsPipeline();

protected:
	//メンバ変数

	//定数バッファ
	ComPtr<ID3D12Resource> constBuffTrans;

	//ローカルスケール
	Vector3 scale = { 1,1,1 };

	//X,Y,Z軸回りのローカル回転角
	Vector3 rotation = { 0,0,0 };

	//ローカル座標
	Vector3 position = { 0,0,0 };

	//ローカルワールド変換行列
	Matrix4 matWorld;

	//定数バッファ(スキン)
	ComPtr<ID3D12Resource> constBuffSkin;


private:

	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;

	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

	CD3DX12_HEAP_PROPERTIES cbheapprop{};

	CD3DX12_RESOURCE_DESC cbresdesc{};
	CD3DX12_RESOURCE_DESC cbresdesc2{};

	//モデル
	FBXModel* fbxModel = nullptr;
 
public:

	//毎フレーム更新
	void Update();

	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//モデルのセット
	void SetModel(FBXModel* fbxModel) { this->fbxModel = fbxModel; }

	//定数
	//ボーンの最大数
	static const int MAX_BONES = 32;

	//定数バッファ用データ構造体(スキニング)
	struct ConstBufferDataSkin
	{
		Matrix4 bones[MAX_BONES];
	};

};