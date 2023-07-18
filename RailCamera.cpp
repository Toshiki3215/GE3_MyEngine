#include "RailCamera.h"

RailCamera::RailCamera(int window_width, int window_height)
{
	aspectRatio = (float)window_width / window_height;

	//�r���[�s��̌v�Z
	UpdateViewMatrix();

	// �ˉe�s��̌v�Z
	UpdateProjectionMatrix();

	// �r���[�v���W�F�N�V�����̍���
	matViewProjection = matView * matProjection;
}

void RailCamera::Initialize(Transform wtf)
{
	matView = wtf.matWorld;

	matViewProjection = ConvertXM::ConvertXMMATtoMat4(DirectX::XMMatrixIdentity());

}

void RailCamera::Update(Transform wtf)
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
	matViewProjection = matView * matProjection;
}

void RailCamera::UpdateViewMatrix()
{

	// ���_���W
	Vector3 eyePosition = eye;
	// �����_���W
	Vector3 targetPosition = target;
	// �i���́j�����
	Vector3 upVector = up;

	// �J����Z���i���������j
	Vector3 cameraAxisZ = targetPosition - eyePosition;

	// 0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O

	// �x�N�g���𐳋K��
	cameraAxisZ.nomalize();

	// �J������X���i�E�����j
	Vector3 cameraAxisX;
	// X���͏������Z���̊O�ςŋ��܂�
	cameraAxisX = upVector.cross(cameraAxisZ);
	// �x�N�g���𐳋K��
	cameraAxisX.nomalize();

	// �J������Y���i������j
	Vector3 cameraAxisY;
	// Y����Z����X���̊O�ςŋ��܂�
	cameraAxisY = cameraAxisZ.cross(cameraAxisX);

	// �����܂łŒ�������3�����̃x�N�g��������
	//�i���[���h���W�n�ł̃J�����̉E�����A������A�O�����j	

	// �J������]�s��

	Matrix4 matCameraRot;
	// �J�������W�n�����[���h���W�n�̕ϊ��s��
	matCameraRot.m[0][0] = cameraAxisX.x;
	matCameraRot.m[0][1] = cameraAxisX.y;
	matCameraRot.m[0][2] = cameraAxisX.z;
	matCameraRot.m[0][3] = 0;
	matCameraRot.m[1][0] = cameraAxisY.x;
	matCameraRot.m[1][1] = cameraAxisY.y;
	matCameraRot.m[1][2] = cameraAxisY.z;
	matCameraRot.m[1][3] = 0;
	matCameraRot.m[2][0] = cameraAxisZ.x;
	matCameraRot.m[2][1] = cameraAxisZ.y;
	matCameraRot.m[2][2] = cameraAxisZ.z;
	matCameraRot.m[2][3] = 0;
	matCameraRot.m[3][0] = 0;
	matCameraRot.m[3][1] = 0;
	matCameraRot.m[3][2] = 0;
	matCameraRot.m[3][3] = 1;

	// �]�u�ɂ��t�s��i�t��]�j���v�Z
	matView = ConvertXM::ConvertXMMATtoMat4(XMMatrixTranspose(ConvertXM::ConvertMat4toXMMAT(matCameraRot)));

	// ���_���W��-1���|�������W
	Vector3 reverseEyePosition = -eyePosition;
	// �J�����̈ʒu���烏�[���h���_�ւ̃x�N�g���i�J�������W�n�j
	float tX = cameraAxisX.dot(reverseEyePosition);	// X����
	float tY = cameraAxisY.dot(reverseEyePosition);	// Y����
	float tZ = cameraAxisZ.dot(reverseEyePosition);	// Z����
	// ��̃x�N�g���ɂ܂Ƃ߂�
	Vector3 translation = { tX,tY,tZ };
	// �r���[�s��ɕ��s�ړ�������ݒ�
	matView.m[3][0] = translation.x;
	matView.m[3][1] = translation.y;
	matView.m[3][2] = translation.z;
	matView.m[3][3] = 1;

#pragma region �S�����r���{�[�h�s��̌v�Z
	// �r���{�[�h�s��
	matBillboard.m[0][0] = cameraAxisX.x;
	matBillboard.m[0][1] = cameraAxisX.y;
	matBillboard.m[0][2] = cameraAxisX.z;
	matBillboard.m[0][3] = 0;
	matBillboard.m[1][0] = cameraAxisY.x;
	matBillboard.m[1][1] = cameraAxisY.y;
	matBillboard.m[1][2] = cameraAxisY.z;
	matBillboard.m[1][3] = 0;
	matBillboard.m[2][0] = cameraAxisZ.x;
	matBillboard.m[2][1] = cameraAxisZ.y;
	matBillboard.m[2][2] = cameraAxisZ.z;
	matBillboard.m[2][3] = 0;
	matBillboard.m[3][0] = 0;
	matBillboard.m[3][1] = 0;
	matBillboard.m[3][2] = 0;
	matBillboard.m[3][3] = 1;
#pragma region

#pragma region Y�����r���{�[�h�s��̌v�Z
	// �J����X���AY���AZ��
	Vector3 ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	// X���͋���
	ybillCameraAxisX = cameraAxisX;
	// Y���̓��[���h���W�n��Y��
	ybillCameraAxisY = upVector.nomalize();
	// Z����X����Y���̊O�ςŋ��܂�
	ybillCameraAxisZ = ybillCameraAxisX.cross(ybillCameraAxisY);

	// Y�����r���{�[�h�s��
	matBillboardY.m[0][0] = cameraAxisX.x;
	matBillboardY.m[0][1] = cameraAxisX.y;
	matBillboardY.m[0][2] = cameraAxisX.z;
	matBillboardY.m[0][3] = 0;
	matBillboardY.m[1][0] = cameraAxisY.x;
	matBillboardY.m[1][1] = cameraAxisY.y;
	matBillboardY.m[1][2] = cameraAxisY.z;
	matBillboardY.m[1][3] = 0;
	matBillboardY.m[2][0] = cameraAxisZ.x;
	matBillboardY.m[2][1] = cameraAxisZ.y;
	matBillboardY.m[2][2] = cameraAxisZ.z;
	matBillboardY.m[2][3] = 0;
	matBillboardY.m[3][0] = 0;
	matBillboardY.m[3][1] = 0;
	matBillboardY.m[3][2] = 0;
	matBillboardY.m[3][3] = 1;
#pragma endregion
}

void RailCamera::UpdateViewMatrix(Vector3 newEye)
{
	XMFLOAT3 xmEye;
	xmEye.x = newEye.x;
	xmEye.y = newEye.y;
	xmEye.z = newEye.z;

	XMFLOAT3 xmTarget;
	xmTarget.x = target.x;
	xmTarget.y = target.y;
	xmTarget.z = target.z;

	XMFLOAT3 xmUp;
	xmUp.x = up.x;
	xmUp.y = up.y;
	xmUp.z = up.z;

	XMMATRIX xmMatView = XMMatrixLookAtLH(
		XMLoadFloat3(&xmEye), XMLoadFloat3(&xmTarget), XMLoadFloat3(&xmUp));

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {

			matView.m[i][j] = xmMatView.r[i].m128_f32[j];

		}
	}
}

void RailCamera::UpdateProjectionMatrix()
{
	// �������e�ɂ��ˉe�s��̐���
	matProjection.MakePerspectiveL(
		FieldOfViewY(),
		aspectRatio,
		0.1f, 100000.0f, matProjection
	);
}

float RailCamera::FieldOfViewY()
{

	return 2.0f * atanf(35 / (2.0f * 50));

}