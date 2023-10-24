// FBXModel.h
// FBXのモデルを管理

#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include "Vector4.h"
#include "Matrix4.h"
#include <fbxsdk.h>


struct Node
{
	//名前
	std::string name;

	//ローカルスケール
	Vector4 scaling = { 1,1,1,0 };

	//ローカル回転角
	Vector4 rotation = { 0,0,0,0 };

	//ローカル移動
	Vector4 translation = { 0,0,0,1 };

	//ローカル変形行列
	Matrix4 transform;

	//グローバル変形行列
	Matrix4 globalTransform;

	//親ノード
	Node* parent = nullptr;

};

class FBXModel
{
public:
	//デストラクタ
	~FBXModel();

	//フレンドクラス
	friend class FBXLoader;

	//ボーン構造体
	struct Bone
	{
		//名前
		std::string name;

		//初期姿勢の逆行列
		Matrix4 invInitialPose;

		//クラスター(FBX側のボーン情報)
		FbxCluster* fbxCluster;

		//コンストラクタ
		Bone(const std::string& name)
		{
			this->name = name;
		}

	};

private:
	//内部クラスのメンバ変数

	//ボーン配列
	std::vector<Bone> bones;

public:
	//getter
	std::vector<Bone>& GetBones() { return bones; }

	//内部クラスの定数

	//ボーンインデックスの最大数
	static const int MAX_BONE_INDICES = 4;


private:

	//モデル名
	std::string name;

	//ノード配列
	std::vector<Node> nodes;

	//アンビエント係数
	DirectX::XMFLOAT3 ambient = { 1,1,1 };

	//ディフューズ係数
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };

	//テクスチャメタデータ
	DirectX::TexMetadata metadata = {};

	//スクラッチイメージ
	DirectX::ScratchImage scratchImg = {};


public:

	//サブクラス

	//頂点データ構造体
	struct VertexPosNormalUvSkin
	{
		//x,y,z座標
		DirectX::XMFLOAT3 pos;

		//法線ベクトル
		DirectX::XMFLOAT3 normal;

		//uv座標
		DirectX::XMFLOAT2 uv;

		//ボーン　番号
		UINT boneIndex[MAX_BONE_INDICES];

		//ボーン　重み
		float boneWeight[MAX_BONE_INDICES];

	};

	//メッシュを持つノード
	Node* meshNode = nullptr;

	//頂点データ配列
	std::vector<VertexPosNormalUvSkin> vertices;

	//頂点インデックス配列
	std::vector<unsigned short> indices;

private:

	//エイリアス
	//Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;

	//std::を省略
	using string = std::string;

	template <class T> using vector = std::vector<T>;

	CD3DX12_HEAP_PROPERTIES heapprop{};
	CD3DX12_RESOURCE_DESC resdesc{};
	CD3DX12_HEAP_PROPERTIES pHeap = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;

	//インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;

	//テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;

	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};

	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};

	//SRV用デスクリプターヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	//FBXシーン
	FbxScene* fbxScene = nullptr;

public:

	//バッファ生成
	void CreateBuffers(ID3D12Device* device);

	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//モデルの変形行列取得
	const Matrix4& GetModelTransform() { return meshNode->globalTransform; }

	//getter
	FbxScene* GetFbxScene() { return fbxScene; }

};