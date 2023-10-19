// Object3D.h
// ���f���̏������A�X�V�����A�`��Ȃǂ̊Ǘ�

#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string.h>
#include "Model.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Affin.h"
#include "Transform.h"
#include "Camera.h"

class Object3d
{
private:

	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataB0
	{
		Matrix4 mat;	// �R�c�ϊ��s��
	};

private:

	static const int division = 50;			// ������
	static const float radius;				// ��ʂ̔��a
	static const float prizmHeight;			// ���̍���
	static const int planeCount = division * 2 + division * 2; // �ʂ̐�
	static const int vertexCount = planeCount * 3;		       // ���_��

public: 

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="device"></param>
	/// <param name="window_width"></param>
	/// <param name="window_height"></param>
	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="cmdList"></param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static Object3d* Create();

	/// <summary>
	/// �������Ă��邩
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return  isDead_; }

private:

	// �f�o�C�X
	static ComPtr<ID3D12Device> device;

	// �R�}���h���X�g
	static ComPtr<ID3D12GraphicsCommandList> cmdList;

	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;

	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;

	// �r���[�s��
	static Matrix4 matView;
	// �ˉe�s��
	static Matrix4 matProjection;
	// ���_���W
	static Vector3 eye;
	// �����_���W
	static Vector3 target;
	// ������x�N�g��
	static Vector3 up;

	static float focalLengs;

private:

	static void InitializeCamera(int window_width, int window_height);

	static void InitializeGraphicsPipeline();

	// �r���[�s����X�V
	static void UpdateViewMatrix();

public:

	Object3d();
	~Object3d();

	//������
	bool Initialize();

	// ���t���[������
	void Update();
	void Update(Transform* parentWtf);

	//�s��̍X�V
	void UpdateMat();

	// �`��
	void Draw();

	/// <summary>
	/// �Q�b�^�[(�e�q�֌W)
	/// </summary>
	/// <returns></returns>
	Object3d* GetParent() const { return parent; }

	/// <summary>
	/// �Z�b�^�[(�e�q�֌W)
	/// </summary>
	/// <param name="parent_"></param>
	void SetParent(Object3d* parent_) { this->parent = parent_; }

	/// <summary>
	/// �Z�b�^�[(�J����)
	/// </summary>
	/// <param name="camera_"></param>
	static void SetCamera(Camera* camera_) { Object3d::camera = camera_; }

	/// <summary>
	/// �Z�b�^�[(���f��)
	/// </summary>
	/// <param name="model_"></param>
	void SetModel(Model* model_) { this->model = model_; }

private: 
	// �����o�ϐ�

public:

	ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@

	bool isDead_ = false;

	// �F
	Vector4 color = { 1,1,1,1 };

	// �e�I�u�W�F�N�g
	Object3d* parent = nullptr;

	//���f��
	Model* model = nullptr;
	static Camera* camera;

	static float win_wi, win_hi;

public:

	Transform wtf;

};