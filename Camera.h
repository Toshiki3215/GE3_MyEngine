#pragma once
#include "Vector3.h"
#include "Matrix4.h"
#include "Affin.h"
#include"Transform.h"
#include <DirectXMath.h>

class Camera
{

public: // �����o�֐�
	
	// �R���X�g���N�^
	Camera(int window_width, int window_height);

	// �f�X�g���N�^
	virtual ~Camera() = default;

	// ���t���[���X�V
	virtual void Update();
	virtual void Update(Transform wtf);

	// �r���[�s����X�V
	void UpdateViewMatrix();
	void UpdateViewMatrix(Vector3 newEye);

	// �ˉe�s����X�V
	void UpdateProjectionMatrix();

	// �r���[�s��̎擾
	inline const Matrix4& GetViewMatrix() 
	{
		return matView;
	}

	// �ˉe�s��̎擾
	inline const Matrix4& GetProjectionMatrix() 
	{
		return matProjection;
	}

	// �r���[�ˉe�s��̎擾
	inline const Matrix4& GetViewProjectionMatrix() 
	{
		return matViewProjection;
	}

	// �r���{�[�h�s��̎擾
	inline const Matrix4& GetBillboardMatrix() 
	{
		return matBillboard;
	}

	// ���_���W�̎擾
	inline const Vector3& GetEye() 
	{
		return eye;
	}

	// ���_���W�̐ݒ�
	inline void SetEye(Vector3 eye) 
	{
		this->eye = eye; viewDirty = true;
	}

	// �����_���W�̎擾
	inline const Vector3& GetTarget() 
	{
		return target;
	}

	// �����_���W�̐ݒ�
	inline void SetTarget(Vector3 target) 
	{
		this->target = target; viewDirty = true;
	}

	// ������x�N�g���̎擾
	inline const Vector3& GetUp() 
	{
		return up;
	}

	// ������x�N�g���̐ݒ�
	inline void SetUp(Vector3 up) 
	{
		this->up = up; viewDirty = true;
	}

	// sensor
	inline const float& GetSensor() 
	{
		return sensor;
	}

	inline void SetSensor(float sensor)
	{
		this->sensor = sensor; viewDirty = true;
	}

	// FL
	inline const float& GetForcalLengs()
	{
		return focalLengs;
	}

	inline void SetFocalLengs(float focalLengs) 
	{
		this->focalLengs = focalLengs; viewDirty = true;
	}


	// �x�N�g���ɂ�鎋�_�ړ�
	void MoveEyeVector(const Vector3& move);

	// �x�N�g���ɂ��ړ�
	void MoveVector(const Vector3& move);

	float FieldOfViewY();

protected: // �����o�ϐ�

	// �r���[�s��
	Matrix4 matView = Affin::matUnit();

	// �r���{�[�h�s��
	Matrix4 matBillboard = Affin::matUnit();

	// Y�����r���{�[�h�s��
	Matrix4 matBillboardY = Affin::matUnit();

	// �ˉe�s��
	Matrix4 matProjection = Affin::matUnit();

	// �r���[�ˉe�s��
	Matrix4 matViewProjection = Affin::matUnit();

	// �r���[�s��_�[�e�B�t���O
	bool viewDirty = false;

	// �ˉe�s��_�[�e�B�t���O
	bool projectionDirty = false;

	// ���_���W
	Vector3 eye = { 0, 0, -1 };

	// �����_���W
	Vector3 target = { 0, 0, 0 };

	// ������x�N�g��
	Vector3 up = { 0, 1, 0 };

	// �A�X�y�N�g��
	float aspectRatio = 1.0f;

	float focalLengs = 50;
	float sensor = 35;
};

