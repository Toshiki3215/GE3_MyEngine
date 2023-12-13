// Enemy.cpp
// Enemyを管理している

#include "Enemy.h"
#include "Matrix4.h"
#include <cassert>

void Enemy::Initilize(Vector3 pos)
{
	//自機
	enemyMD = Model::LoadFromOBJ("obj3");
	enemyObj = Object3d::Create();
	enemyObj->SetModel(enemyMD);
	enemyObj->wtf.position = pos;
	enemyObj->wtf.scale = { 1.0f,1.0f,1.0f };

	enemyBulletModel_ = Model::LoadFromOBJ("obj4");
	enemyBulletObj_ = Object3d::Create();
	enemyBulletObj_->SetModel(enemyBulletModel_);
	enemyBulletObj_->wtf.position = { enemyObj->wtf.position.x,enemyObj->wtf.position.y + 0.07f, enemyObj->wtf.position.z };
	enemyBulletObj_->wtf.scale = { 0.5f,0.5f,0.5f };

	phase_ = Phase::Approch;
}

void Enemy::Update(Vector3 pos)
{

	len = pos - enemyObj->wtf.position;
	len.nomalize();

	switch (phase_)
	{
	case Phase::Approch:

		//enemyObj->wtf.position -= enemySpeed;
		//デスフラグの立った弾を削除
		enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });

		if (shootFlg == true)
		{
			bulletSpeed = 0.05f;

			if (--coolTimer <= 0)
			{
				shotCool = false;
			}

			if (shotCool == false)
			{
				//弾生成し、初期化
				std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
				newBullet->Initilize(enemyObj);

				//弾を登録する
				enemyBullets_.push_back(std::move(newBullet));

				shotCool = true;
				coolTimer = 300;
			}
		}

		//弾更新
		for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_)
		{
			bullet->Update(len, bulletSpeed, enemyObj);
		}

		break;

	case Phase::Leave:

		break;

	case Phase::Death:

		deathTimer--;
		float rSpeed = 3.14f / 10;
		enemyObj->wtf.rotation.y -= rSpeed;

		if (deathTimer <= 0)
		{
			deathTimer = 20;
			isDead = true;
			isClear = TRUE;
		}

		break;
	}

	enemyObj->Update();

}

void Enemy::Draw()
{
	if (phase_ == Phase::Approch)
	{
		enemyObj->Draw();

		for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_)
		{
			bullet->Draw();
		}
	}
}

void Enemy::OnCollision()
{

	phase_ = Phase::Death;

}

Vector3 Enemy::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;

	enemyObj->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	worldPos.x = enemyObj->wtf.matWorld.m[3][0];
	worldPos.y = enemyObj->wtf.matWorld.m[3][1];
	worldPos.z = enemyObj->wtf.matWorld.m[3][2];

	return worldPos;
}

void Enemy::SetParentCamera(Vector3 cameraWtf)
{
	enemyObj->wtf.position.z = cameraWtf.z + 60;
}

void Enemy::reset()
{
	isClear = FALSE;
}
