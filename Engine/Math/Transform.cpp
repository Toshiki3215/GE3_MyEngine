// Transform.cpp
// ワールド座標(WorldTransform)の作成

#include "Transform.h"

Transform::Transform() 
{
	// ローカルスケール
	scale = { 1,1,1 };

	// X,Y,Z軸回りのローカル回転角
	rotation = { 0,0,0 };

	// ローカル座標
	position = { 0,0,0 };

	// ローカルワールド変換行列
	matWorld = Affin::matUnit();

	//親となるワールド変換へのポインタ
	parent_ = nullptr;

}

Transform::~Transform() {}

void Transform::Initialize() 
{
	// ローカルスケール
	scale = { 1,1,1 };

	// X,Y,Z軸回りのローカル回転角
	rotation = { 0,0,0 };

	// ローカル座標
	position = { 0,0,0 };

	// ローカルワールド変換行列
	matWorld = Affin::matUnit();

	//親となるワールド変換へのポインタ
	parent_ = nullptr;
}

void Transform::UpdateMat() 
{
	Matrix4 matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = Affin::matScale(scale.x, scale.y, scale.z);
	matRot = Affin::matUnit();
	matRot *= Affin::matRotation(rotation);
	matTrans = Affin::matTrans(position.x, position.y, position.z);

	// ワールド行列の合成
	matWorld = Affin::matUnit(); // 変形をリセット
	matWorld *= matScale; // ワールド行列にスケーリングを反映
	matWorld *= matRot; // ワールド行列に回転を反映
	matWorld *= matTrans; // ワールド行列に平行移動を反映

	// 親行列の指定がある場合は、掛け算する
	if (parent_) 
	{
		matWorld *= parent_->matWorld;
	}

}