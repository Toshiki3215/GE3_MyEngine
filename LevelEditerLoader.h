#pragma once

#include <string>
#include <vector>
#include <DirectXMath.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"

//���x���G�f�B�^
struct LevelEData 
{
	struct ObjData
	{
		//�t�@�C����
		std::string fileName;

		//���s�ړ�
		Vector3 translation;
		//DirectX::XMVECTOR translation;

		//��]�p
		Vector3 rotation;
		//DirectX::XMVECTOR rotation;

		//�X�P�[�����O
		Vector3 scaling;
		//DirectX::XMVECTOR scaling;

	};

	//�I�u�W�F�N�g�z��
	std::vector<ObjData> objects;

};

class LevelELoader
{
public:
	//�萔
	//�f�t�H���g�̓ǂݍ��݃f�B���N�g��
	static const std::string kDefaultBaseDirectory;

	//�t�@�C���g���q
	static const std::string kExtension;

public:
	//�����o�֐�
	//���x���f�[�^�t�@�C���̓ǂݍ���
	static LevelEData* LoadFile(const std::string& fileName);

};