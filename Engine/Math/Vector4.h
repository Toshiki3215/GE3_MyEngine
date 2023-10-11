#pragma once

class Vector4
{
public:
	float x;//x����
	float y;//y����
	float z;//z����
	float w;//w

public:
	//�R���X�g���N�^
	Vector4();//��x�N�g��
	Vector4(float x, float y, float z, float w);//�������w�肵�Đ���

	float Length()const;//�m���������߂�
	float LengthSquared()const;
	Vector4 Normalization()const;//���K������
	Vector4& Normal();//���K������
	float Dot(const Vector4& v)const;//���ς����߂�
	Vector4 Cross(const Vector4& v)const;//�O�ς����߂�

	//�P�����Z�q�I�[�o�[���[�h
	Vector4 operator+() const;
	Vector4 operator-() const;

	// ������Z�q�I�[�o�[���[�h
	Vector4& operator+=(const Vector4& v);
	Vector4& operator-=(const Vector4& v);
	Vector4& operator*=(float s);
	Vector4& operator/=(float s);
};

//2�����Z�q�I�[�o�[���[�h
const Vector4 operator+(const Vector4& v1, const Vector4& v2);

const Vector4 operator-(const Vector4& v1, const Vector4& v2);
const Vector4 operator*(const Vector4& v, float s);
const Vector4 operator*(float s, const Vector4& v);
const Vector4 operator/(const Vector4& v, float s);

void Vector4Lerp(const Vector4& src1, const Vector4& src2, float t, Vector4& dest);

Vector4 Vector4Lerp(const Vector4& src1, const Vector4& src2, float t);