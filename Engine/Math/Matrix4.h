// Matrix4.h
// 自作の数学用ヘッダー(行列)

#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include <DirectXMath.h>

class Matrix4
{
public:

	float m[4][4];

public:

	// コンストラクタ
	Matrix4();
	Matrix4(float num);

	// 成分を指定しての生成
	Matrix4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	// 代入演算子オーバーロード
	static void MakeOrthogonalL(float left, float right, float bottom, float top, float near_, float far_, Matrix4& matrix);
	static void MakeLookL(const Vector3& eye, const Vector3& target, const Vector3& up, Matrix4& mat);
	static void MakePerspectiveL(float fovAngleY, float aspect, float near_, float far_, Matrix4& matrix);
	Matrix4 MakeInverse(const Matrix4* mat);

	static Matrix4 MakeIdentity();

	Vector3 transform(const Vector3& v, const Matrix4& m);
};

// 代入演算子オーバーロード
Matrix4& operator*=(Matrix4& m1, const Matrix4& m2);

// 2項演算子オーバーロード
const Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
const Vector3 operator*(const Vector3& v, const Matrix4& m2);

//Vector3 ver
Matrix4 ScaleMatrix4(Vector3 scale);
Matrix4 RotationXMatrix4(Vector3 rotation);
Matrix4 RotationYMatrix4(Vector3 rotation);
Matrix4 RotationZMatrix4(Vector3 rotation);
Matrix4 MoveMatrix4(Vector3 translation);

//Vector4 ver
Matrix4 ScaleMatrix4(Vector4 scale);
Matrix4 RotationXMatrix4(Vector4 rotation);
Matrix4 RotationYMatrix4(Vector4 rotation);
Matrix4 RotationZMatrix4(Vector4 rotation);
Matrix4 MoveMatrix4(Vector4 translation);

Matrix4 RotationMatrix4(Vector4 rotation);