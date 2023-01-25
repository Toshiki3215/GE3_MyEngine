#pragma once
#include<DirectXMath.h>

class SpriteCommon
{
private: 

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;


public:

	//���_�f�[�^�\����
	struct Vertex
	{
		XMFLOAT3 pos;    //x,y,z���W
		XMFLOAT3 normal; //�@���x�N�g��
		XMFLOAT2 uv;     //u,v���W
	};

	//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
	struct ConstBufferDataMaterial
	{
		//�F(RGBA)
		XMFLOAT4 color;
	};

	//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
	struct ConstBufferDataTransform
	{
		//3D�ϊ��s��
		XMMATRIX mat;
	};

	void Initialize();

};

