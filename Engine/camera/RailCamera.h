// RailCamera.h
// 自作のカメラ用ヘッダー(レールカメラ)

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

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="wtf"></param>
	void Initialize(Transform wtf);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="wtf"></param>
	void Update(Transform wtf);

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	void UpdateViewMatrix();

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	/// <param name="newEye"></param>
	void UpdateViewMatrix(Vector3 newEye);

	/// <summary>
	/// 射影行列を更新
	/// </summary>
	void UpdateProjectionMatrix();


	float FieldOfViewY();

	/// <summary>
	/// 視点座標の設定
	/// </summary>
	/// <param name="eye"></param>
	inline void SetEye(Vector3 eye)
	{
		this->eye = eye;
		viewDirty = true;
	}

	/// <summary>
	/// 注視点座標の設定
	/// </summary>
	/// <param name="eye"></param>
	inline void SetTarget(Vector3 target)
	{
		this->target = target;
		viewDirty = true;
	}

	/// <summary>
	/// 視点座標の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetEye()
	{
		return eye;
	}

	/// <summary>
	/// 注視点座標の取得
	/// </summary>
	/// <returns></returns>
	Transform GetWtf()
	{
		return railCameraWtf;
	}

private:

	// ビュー行列
	Matrix4 matView = ConvertXM::ConvertXMMATtoMat4(DirectX::XMMatrixIdentity());
	// ビルボード行列
	Matrix4 matBillboard = ConvertXM::ConvertXMMATtoMat4(DirectX::XMMatrixIdentity());
	// Y軸回りビルボード行列
	Matrix4 matBillboardY = ConvertXM::ConvertXMMATtoMat4(DirectX::XMMatrixIdentity());
	// 射影行列
	Matrix4 matProjection = ConvertXM::ConvertXMMATtoMat4(DirectX::XMMatrixIdentity());
	// ビュー射影行列
	Matrix4 matViewProjection;

	// 視点座標
	Vector3 eye = { 0, 0, -1 };

	// 注視点座標
	Vector3 target = { 0, 0, 0 };

	// 上方向ベクトル
	Vector3 up = { 0, 1, 0 };

	// アスペクト比
	float aspectRatio = 1.0f;

	// ビュー行列ダーティフラグ
	bool viewDirty = false;

	Transform railCameraWtf;

};