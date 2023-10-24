// Collider.h
// ゲームシーンの当たり判定を管理している

#pragma once

class Collider
{
private:

	//衝突半径
	float radius = 10;

public:

	//半径を取得
	float GetRadius() { return radius; }

	// 半径を設定
	void SetRadius(float r) { radius = r; }

	//衝突時に呼ばれる関数
	void OnCollision();

};