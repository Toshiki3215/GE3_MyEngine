#pragma once
#include "Vector3.h"
#include "Matrix4.h"
#include "Affin.h"
#include"Transform.h"
#include <DirectXMath.h>

class Camera
{

public: // メンバ関数
	
	// コンストラクタ
	Camera(int window_width, int window_height);

	// デストラクタ
	virtual ~Camera() = default;

	// 毎フレーム更新
	virtual void Update();
	virtual void Update(Transform wtf);

	// ビュー行列を更新
	void UpdateViewMatrix();
	void UpdateViewMatrix(Vector3 newEye);

	// 射影行列を更新
	void UpdateProjectionMatrix();

	// ビュー行列の取得
	inline const Matrix4& GetViewMatrix() 
	{
		return matView;
	}

	// 射影行列の取得
	inline const Matrix4& GetProjectionMatrix() 
	{
		return matProjection;
	}

	// ビュー射影行列の取得
	inline const Matrix4& GetViewProjectionMatrix() 
	{
		return matViewProjection;
	}

	// ビルボード行列の取得
	inline const Matrix4& GetBillboardMatrix() 
	{
		return matBillboard;
	}

	// 視点座標の取得
	inline const Vector3& GetEye() 
	{
		return eye;
	}

	// 視点座標の設定
	inline void SetEye(Vector3 eye) 
	{
		this->eye = eye; viewDirty = true;
	}

	// 注視点座標の取得
	inline const Vector3& GetTarget() 
	{
		return target;
	}

	// 注視点座標の設定
	inline void SetTarget(Vector3 target) 
	{
		this->target = target; viewDirty = true;
	}

	// 上方向ベクトルの取得
	inline const Vector3& GetUp() 
	{
		return up;
	}

	// 上方向ベクトルの設定
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


	// ベクトルによる視点移動
	void MoveEyeVector(const Vector3& move);

	// ベクトルによる移動
	void MoveVector(const Vector3& move);

	float FieldOfViewY();

protected: // メンバ変数

	// ビュー行列
	Matrix4 matView = Affin::matUnit();

	// ビルボード行列
	Matrix4 matBillboard = Affin::matUnit();

	// Y軸回りビルボード行列
	Matrix4 matBillboardY = Affin::matUnit();

	// 射影行列
	Matrix4 matProjection = Affin::matUnit();

	// ビュー射影行列
	Matrix4 matViewProjection = Affin::matUnit();

	// ビュー行列ダーティフラグ
	bool viewDirty = false;

	// 射影行列ダーティフラグ
	bool projectionDirty = false;

	// 視点座標
	Vector3 eye = { 0, 0, -1 };

	// 注視点座標
	Vector3 target = { 0, 0, 0 };

	// 上方向ベクトル
	Vector3 up = { 0, 1, 0 };

	// アスペクト比
	float aspectRatio = 1.0f;

	float focalLengs = 50;
	float sensor = 35;
};

