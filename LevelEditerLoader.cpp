#include "LevelEditerLoader.h"

//#include <json.hpp>
#include "json.hpp"
#include <fstream>
#include <cassert>

const std::string LevelELoader::kDefaultBaseDirectory = "Resources/levels/";
const std::string LevelELoader::kExtension = ".json";

LevelEData* LevelELoader::LoadFile(const std::string& fileName)
{
	//�A�����ăt���p�X�𓾂�
	const std::string fullpath = kDefaultBaseDirectory + fileName + kExtension;

	//�t�@�C���X�g���[��
	std::ifstream file;

	//�t�@�C�����J��
	file.open(fullpath);

	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	//JSON�����񂩂�𓀂����f�[�^
	nlohmann::json deserialized;

	//��
	file >> deserialized;

	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"�𕶎���Ƃ��Ď擾
	std::string name = deserialized["name"].get<std::string>();

	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);

	//���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	LevelEData* levelEData = new LevelEData();

	// "objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));

		//��ʂ��擾
		std::string type = object["type"].get<std::string>();

		// ----- ��ނ��Ƃ̏��� ----- //

		//MESH
		if (type.compare("MESH") == 0)
		{
			//�v�f�ǉ�
			levelEData->objects.emplace_back(LevelEData::ObjData{});

			//���ǉ������v�f�̎Q�Ƃ𓾂�
			LevelEData::ObjData& objData = levelEData->objects.back();

			if (object.contains("file_name"))
			{
				//�t�@�C����
				objData.fileName = object["file_name"];
			}

			//�g�����X�t�H�[���̃p�����[�^�ǂݍ���
			nlohmann::json& transform = object["transform"];

			//���s�ړ�
			objData.translation.x =  (float)transform["translation"][1];
			objData.translation.y =  (float)transform["translation"][2];
			objData.translation.z = -(float)transform["translation"][0];

			//��]�p
			objData.rotation.x = -(float)transform["rotation"][1];
			objData.rotation.y = -(float)transform["rotation"][2];
			objData.rotation.z =  (float)transform["rotation"][0];

			//�X�P�[�����O
			objData.scaling.x = (float)transform["scaling"][1];
			objData.scaling.y = (float)transform["scaling"][2];
			objData.scaling.z = (float)transform["scaling"][0];

			// TODO: �R���C�_�[�̃p�����[�^�ǂݍ���

		}


		// TODO: �I�u�W�F�N�g�������ċA�֐��ɂ܂Ƃ߁A�ċA�ďo�Ŏ}�𑖍�����
		if (object.contains("children")) 
		{
			
		}
	}

	return levelEData;

}