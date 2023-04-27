#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>

struct Node
{
	//���O
	std::string name;

	//���[�J���X�P�[��
	DirectX::XMVECTOR scaling = { 1,1,1,0 };

	//���[�J����]�p
	DirectX::XMVECTOR rotation = { 0,0,0,0 };

	//���[�J���ړ�
	DirectX::XMVECTOR translation = { 0,0,0,1 };

	//���[�J���ό`�s��
	DirectX::XMMATRIX transform;

	//�O���[�o���ό`�s��
	DirectX::XMMATRIX globalTransform;

	//�e�m�[�h
	Node* parent = nullptr;

};

class FBXModel
{
public:

	//�t�����h�N���X
	friend class FBXLoader;

private:

	//���f����
	std::string name;

	//�m�[�h�z��
	std::vector<Node> nodes;

	//�A���r�G���g�W��
	DirectX::XMFLOAT3 ambient = { 1,1,1 };

	//�f�B�t���[�Y�W��
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };

	//�e�N�X�`�����^�f�[�^
	DirectX::TexMetadata metadata = {};

	//�X�N���b�`�C���[�W
	DirectX::ScratchImage scratchImg = {};


public:

	//�T�u�N���X

	//���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		//x,y,z���W
		DirectX::XMFLOAT3 pos;

		//�@���x�N�g��
		DirectX::XMFLOAT3 normal;

		//uv���W
		DirectX::XMFLOAT2 uv;

	};

	//���b�V�������m�[�h
	Node* meshNode = nullptr;

	//���_�f�[�^�z��
	std::vector<VertexPosNormalUv> vertices;

	//���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;

private:

	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;

	//std::���ȗ�
	using string = std::string;

	template <class T> using vector = std::vector<T>;

	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;

	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;

	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff;

	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};

	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};

	//SRV�p�f�X�N���v�^�[�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

public:

	//�o�b�t�@����
	void CreateBuffers(ID3D12Device* device);

	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//���f���̕ό`�s��擾
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }

};