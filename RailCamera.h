#pragma once
#include "Vector3.h"
#include "Matrix4.h"
#include "Affin.h"
#include"Transform.h"
#include "ConvertXM.h"

class RailCamera
{
public:
	RailCamera(int window_width, int window_height);

	void Initialize(Transform wtf);

	void Update(Transform wtf);

	void UpdateViewMatrix();

	void UpdateViewMatrix(Vector3 newEye);

	void UpdateProjectionMatrix();

	float FieldOfViewY();

	inline void SetEye(Vector3 eye)
	{
		this->eye = eye; 
		viewDirty = true;
	}

	inline void SetTarget(Vector3 target)
	{
		this->target = target;
		viewDirty = true;
	}

private:

	// �r���[�s��
	Matrix4 matView = ConvertXM::ConvertXMMATtoMat4(DirectX::XMMatrixIdentity());
	// �r���{�[�h�s��
	Matrix4 matBillboard = ConvertXM::ConvertXMMATtoMat4(DirectX::XMMatrixIdentity());
	// Y�����r���{�[�h�s��
	Matrix4 matBillboardY = ConvertXM::ConvertXMMATtoMat4(DirectX::XMMatrixIdentity());
	// �ˉe�s��
	Matrix4 matProjection = ConvertXM::ConvertXMMATtoMat4(DirectX::XMMatrixIdentity());
	// �r���[�ˉe�s��
	Matrix4 matViewProjection;

	// ���_���W
	Vector3 eye = { 0, 0, -1 };

	// �����_���W
	Vector3 target = { 0, 0, 0 };

	// ������x�N�g��
	Vector3 up = { 0, 1, 0 };

	// �A�X�y�N�g��
	float aspectRatio = 1.0f;

	// �r���[�s��_�[�e�B�t���O
	bool viewDirty = false;

};