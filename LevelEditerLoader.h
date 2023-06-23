#pragma once

#include <string>
#include <vector>
#include <DirectXMath.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"

//レベルエディタ
struct LevelEData 
{
	struct ObjData
	{
		//ファイル名
		std::string fileName;

		//平行移動
		Vector3 translation;
		//DirectX::XMVECTOR translation;

		//回転角
		Vector3 rotation;
		//DirectX::XMVECTOR rotation;

		//スケーリング
		Vector3 scaling;
		//DirectX::XMVECTOR scaling;

	};

	//オブジェクト配列
	std::vector<ObjData> objects;

};

class LevelELoader
{
public:
	//定数
	//デフォルトの読み込みディレクトリ
	static const std::string kDefaultBaseDirectory;

	//ファイル拡張子
	static const std::string kExtension;

public:
	//メンバ関数
	//レベルデータファイルの読み込み
	static LevelEData* LoadFile(const std::string& fileName);

};