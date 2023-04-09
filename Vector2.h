#pragma once
class Vector2
{
public:
	float x;	//x����
	float y;	//y����

public:
	//�@�R���X�g���N�^
	Vector2();					//��x�N�g���Ƃ��Đ���
	Vector2(float x, float y);		//x�����A���������w�肵�Ă̐���

	//�@�����o�֐�
	float length() const;				//�m�����i�����j�����߂�
	Vector2& nomalize();				//���K������
	float dot(const Vector2& v) const;	//���ς����߂�
	float cross(const Vector2& v) const;//�O�ς����߂�

	//�@�P�����Z�q�I�[�o�[���[�h
	Vector2 operator+() const;
	Vector2 operator-() const;

	//�@������Z�q�I�[�o�[���[�h
	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(float s);
	Vector2& operator/=(float s);
};

//�񍀉��Z�q�I�[�o�[���[�h
const Vector2 operator+(const Vector2& v1, const Vector2& v2);
const Vector2 operator-(const Vector2& v1, const Vector2& v2);
const Vector2 operator*(const Vector2& v, float s);
const Vector2 operator*(float s, const Vector2& v);
const Vector2 operator/(const Vector2& v, float s);