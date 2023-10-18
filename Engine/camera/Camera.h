// Camera.h
// ����̃J�����p�w�b�_�[

#pragma once

#include "Vector3.h"
#include "Matrix4.h"
#include "Affin.h"
#include"Transform.h"
#include "ConvertXM.h"

/// <summary>
/// �J������{�@�\
/// </summary>
class Camera
{

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="window_width">��ʕ�</param>
	/// <param name="window_height">��ʍ���</param>
	Camera(int window_width, int window_height);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Camera() = default;

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	virtual void Update();

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	virtual void Update(Transform wtf);

	/// <summary>
	/// �r���[�s����X�V
	/// </summary>
	void UpdateViewMatrix();

	/// <summary>
	/// �r���[�s����X�V
	/// </summary>
	void UpdateViewMatrix(Vector3 newEye);

	/// <summary>
	/// �ˉe�s����X�V
	/// </summary>
	void UpdateProjectionMatrix();

	/// <summary>
	/// �r���[�s��̎擾
	/// </summary>
	/// <returns>�r���[�s��</returns>
	inline const Matrix4& GetViewMatrix() 
	{
		return matView;
	}

	/// <summary>
	/// �ˉe�s��̎擾
	/// </summary>
	/// <returns>�ˉe�s��</returns>
	inline const Matrix4& GetProjectionMatrix() 
	{
		return matProjection;
	}

	/// <summary>
	/// �r���[�ˉe�s��̎擾
	/// </summary>
	/// <returns>�r���[�ˉe�s��</returns>
	inline const Matrix4& GetViewProjectionMatrix() 
	{
		return matViewProjection;
	}

	/// <summary>
	/// �r���{�[�h�s��̎擾
	/// </summary>
	/// <returns>�r���{�[�h�s��</returns>
	inline const Matrix4& GetBillboardMatrix() 
	{
		return matBillboard;
	}

	/// <summary>
	/// ���_���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	inline const Vector3& GetEye() 
	{
		return eye;
	}

	/// <summary>
	/// ���_���W�̐ݒ�
	/// </summary>
	/// <param name="eye">���W</param>
	inline void SetEye(Vector3 eye_) 
	{
		this->eye = eye_; viewDirty = true;
	}

	/// <summary>
	/// �����_���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	inline const Vector3& GetTarget() 
	{
		return target;
	}

	/// <summary>
	/// �����_���W�̐ݒ�
	/// </summary>
	/// <param name="target">���W</param>
	inline void SetTarget(Vector3 target_) 
	{
		this->target = target_; viewDirty = true;
	}

	/// <summary>
	/// ������x�N�g���̎擾
	/// </summary>
	/// <returns>������x�N�g��</returns>
	inline const Vector3& GetUp() 
	{
		return up;
	}

	/// <summary>
	/// ������x�N�g���̐ݒ�
	/// </summary>
	/// <param name="up">������x�N�g��</param>
	inline void SetUp(Vector3 up_) 
	{
		this->up = up_; viewDirty = true;
	}

	// sensor
	inline const float& GetSensor() 
	{
		return sensor;
	}
	inline void SetSensor(float sensor_) 
	{
		this->sensor = sensor_; viewDirty = true;
	}
	// FL
	inline const float& GetForcalLengs() 
	{
		return focalLengs;
	}
	inline void SetFocalLengs(float focalLengs_) 
	{
		this->focalLengs = focalLengs_; viewDirty = true;
	}


	/// <summary>
	/// �x�N�g���ɂ�鎋�_�ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	void MoveEyeVector(const Vector3& move);

	/// <summary>
	/// �x�N�g���ɂ�钍���_�ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	void MoveTargetVector(const Vector3& move);

	/// <summary>
	/// �x�N�g���ɂ��ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	void MoveVector(const Vector3& move);

	float FieldOfViewY();

protected: // �����o�ϐ�
	// �r���[�s��
	Matrix4 matView = ConvertXM::ConvertXMMATtoMat4(DirectX::XMMatrixIdentity());
	// �r���{�[�h�s��
	Matrix4 matBillboard = ConvertXM::ConvertXMMATtoMat4(DirectX::XMMatrixIdentity());
	// Y�����r���{�[�h�s��
	Matrix4 matBillboardY = ConvertXM::ConvertXMMATtoMat4(DirectX::XMMatrixIdentity());
	// �ˉe�s��
	Matrix4 matProjection = ConvertXM::ConvertXMMATtoMat4(DirectX::XMMatrixIdentity());
	// �r���[�ˉe�s��
	Matrix4 matViewProjection = ConvertXM::ConvertXMMATtoMat4(DirectX::XMMatrixIdentity());
	// �r���[�s��_�[�e�B�t���O
	bool viewDirty = false;
	// �ˉe�s��_�[�e�B�t���O
	bool projectionDirty = false;
	// ���_���W
	Vector3 eye = { 0, 0, 0 };
	// �����_���W
	Vector3 target = { 0, 0, 30 };
	// ������x�N�g��
	Vector3 up = { 0, 1, 0 };
	// �A�X�y�N�g��
	float aspectRatio = 1.0f;

	float focalLengs = 50;
	float sensor = 35;
};
