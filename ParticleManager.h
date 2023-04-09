#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>

class ParticleManager
{
private:

	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// �萔�o�b�t�@�p�f�[�^�\���́i�}�e���A���j
	struct ConstBufferDataMaterial
	{
		XMFLOAT4 color; // �F (RGBA)
	};

	// ���_�f�[�^�\����
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz���W
		float scale;
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMMATRIX mat;
		XMMATRIX matBillboard;	// �r���{�[�h�s��
	};

	//�p�[�e�B�N���ꗱ
	struct Particle 
	{
		using XMFLOAT3 = DirectX::XMFLOAT3;

		//���W
		XMFLOAT3 position = {};
		//���x
		XMFLOAT3 velocity = {};
		//�����x
		XMFLOAT3 accel = {};

		//���݃t���[��
		int frame = 0;
		//�ߋ��t���[��
		int num_frame = 0;

		//�X�P�[��
		float scale = 1.0f;
		//�����l
		float s_scale = 1.0f;
		//�ŏI�l
		float e_scale = 0.0f;

		XMFLOAT4 color;
	};

private:
	static const int division = 50;	// ������
	static const float radius;		// ��ʂ̔��a
	static const float prizmHeight;	// ���̍���
	static const int planeCount = division * 2 + division * 2;// �ʂ̐�
	static const int vertexCount = 1024;

public:

	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	// �`��㏈��
	static void PostDraw();

	// 3D�I�u�W�F�N�g����
	static ParticleManager* Create();

	// ���_���W�̎擾
	static const XMFLOAT3& GetEye() { return eye; }

	// ���_���W�̐ݒ�
	static void SetEye(XMFLOAT3 eye);

	// �����_���W�̎擾
	static const XMFLOAT3& GetTarget() { return target; }

	// �����_���W�̐ݒ�
	static void SetTarget(XMFLOAT3 target);

	// �x�N�g���ɂ�鎋�_�ړ�
	static void CameraMoveEyeVector(XMFLOAT3 move);

private:

	// �f�o�C�X
	static ID3D12Device* device;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// �f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// ���_�o�b�t�@
	static ComPtr<ID3D12Resource> vertBuff;
	// �e�N�X�`���o�b�t�@
	static ComPtr<ID3D12Resource> texbuff;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// �r���[�s��
	static XMMATRIX matView;
	// �ˉe�s��
	static XMMATRIX matProjection;
	// ���_���W
	static XMFLOAT3 eye;
	// �����_���W
	static XMFLOAT3 target;
	// ������x�N�g��
	static XMFLOAT3 up;
	// ���_�o�b�t�@�r���[
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// ���_�f�[�^�z��
	static VertexPos vertices[vertexCount];
	//�r���{�[�h�s��
	static XMMATRIX matBillboard;
	//Y�����r���{�[�h�s��
	static XMMATRIX matBillboardY;
	//�p�[�e�B�N���z��
	std::forward_list<Particle>particles;

private:

	// �f�X�N���v�^�q�[�v�̏�����
	static void InitializeDescriptorHeap();

	// �J����������
	static void InitializeCamera(int window_width, int window_height);

	// �O���t�B�b�N�p�C�v���C������
	static void InitializeGraphicsPipeline();

	// �e�N�X�`���ǂݍ���
	static void LoadTexture();

	// ���f���쐬
	static void CreateModel();

	// �r���[�s����X�V
	static void UpdateViewMatrix();

public:

	bool Initialize();

	// ���t���[������
	void Update();

	// �`��
	void Draw();

	// �p�[�e�B�N���̒ǉ�
	void Add(int life, XMFLOAT3 position, XMFLOAT3 velociy, XMFLOAT3 accel, float start_scale, float end_scale, XMFLOAT4 color);

private:

	ComPtr<ID3D12Resource> constBuff; // �萔�o�b�t�@

	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };

	ConstBufferDataMaterial* constMapMaterial = nullptr;
};
