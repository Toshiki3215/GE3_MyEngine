#pragma once

#include "fbxsdk.h"
#include "FBXModel.h"

#include <d3d12.h>
#include <d3dx12.h>

#include <string>

class FBXLoader
{
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FBXLoader* GetInstance();

	//初期化
	void Initialize(ID3D12Device* device);

	//後始末
	void Finalize();

private:
	// privateなコンストラクタ（シングルトンパターン）
	FBXLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FBXLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FBXLoader(const FBXLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FBXLoader& obj) = delete;

	//D3D12デバイス
	ID3D12Device* device = nullptr;

	//FBXマネージャ	---	FBX SDKを使うのに必要
	FbxManager* fbxManager = nullptr;

	//FBXコンバータ	---	.fbxファイルを読み込むのに必要
	FbxImporter* fbxImporter = nullptr;

	//std::を省略
	using string = std::string;

public:

	//モデル格納ルートパス
	static const string baseDirectory;

	//テクスチャがない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;

	//ファイルからFBXモデル読み込み
	void LoadModelFronmFile(const string& modelName);

	//再帰的にノード構成を解析
	void ParseNodeRecursive(FBXModel* fbxModel, FbxNode* fbxNode, Node* parent = nullptr);

	//メッシュ読み取り
	void ParseMesh(FBXModel* fbxModel, FbxNode* fbxNode);

	//頂点座標読み取リ
	void ParseMeshVertices(FBXModel* fbxModel, FbxMesh* fbxMesh);

	//面情報読み取り
	void ParseMeshFaces(FBXModel* fbxModel, FbxMesh* fbxMesh);

	//マテリアル読み取り
	void ParseMaterial(FBXModel* fbxModel, FbxNode* fbxNode);

	//テクスチャ読み込み
	void LoadTexture(FBXModel* fbxModel, const std::string& fullpath);

	//ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName(const std::string& path);

};