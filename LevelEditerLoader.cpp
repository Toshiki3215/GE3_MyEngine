#include "LevelEditerLoader.h"

//#include <json.hpp>
#include "json.hpp"
#include <fstream>
#include <cassert>

const std::string LevelELoader::kDefaultBaseDirectory = "Resources/levels/";
const std::string LevelELoader::kExtension = ".json";

LevelEData* LevelELoader::LoadFile(const std::string& fileName)
{
	//連結してフルパスを得る
	const std::string fullpath = kDefaultBaseDirectory + fileName + kExtension;

	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullpath);

	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();

	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンスを生成
	LevelEData* levelEData = new LevelEData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		// ----- 種類ごとの処理 ----- //

		//MESH
		if (type.compare("MESH") == 0)
		{
			//要素追加
			levelEData->objects.emplace_back(LevelEData::ObjData{});

			//今追加した要素の参照を得る
			LevelEData::ObjData& objData = levelEData->objects.back();

			if (object.contains("file_name"))
			{
				//ファイル名
				objData.fileName = object["file_name"];
			}

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];

			//平行移動
			objData.translation.x =  (float)transform["translation"][1];
			objData.translation.y =  (float)transform["translation"][2];
			objData.translation.z = -(float)transform["translation"][0];

			//回転角
			objData.rotation.x = -(float)transform["rotation"][1];
			objData.rotation.y = -(float)transform["rotation"][2];
			objData.rotation.z =  (float)transform["rotation"][0];

			//スケーリング
			objData.scaling.x = (float)transform["scaling"][1];
			objData.scaling.y = (float)transform["scaling"][2];
			objData.scaling.z = (float)transform["scaling"][0];

			// TODO: コライダーのパラメータ読み込み

		}


		// TODO: オブジェクト走査を再帰関数にまとめ、再帰呼出で枝を走査する
		if (object.contains("children")) 
		{
			
		}
	}

	return levelEData;

}