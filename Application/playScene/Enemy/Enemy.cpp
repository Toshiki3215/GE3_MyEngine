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

	shootModel_ = Model::LoadFromOBJ("obj4");
	shootObj_ = Object3d::Create();
	shootObj_->SetModel(shootModel_);
	shootObj_->wtf.position = { enemyObj->wtf.position.x,enemyObj->wtf.position.y + 0.07f, enemyObj->wtf.position.z };
	shootObj_->wtf.scale = { 0.5f,0.5f,0.5f };
}

void Enemy::Update(Vector3 pos)
{
	phase_ = Phase::Approch;

	len = pos - enemyObj->wtf.position;
	len.nomalize();

	switch (phase_)
	{
	case Phase::Approch:

		//enemyObj->wtf.position -= enemySpeed;

		//デスフラグの立った弾を削除
		enemyBullets_.remove_if([](EnemyBullet* bullet) { return bullet->IsDead(); });

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
				EnemyBullet* newBullet = new EnemyBullet();
				newBullet->Initilize(enemyObj);

				//弾を登録する
				enemyBullets_.push_back(newBullet);

				shotCool = true;
				coolTimer = 60;
			}
		}

		//弾更新
		for (EnemyBullet* bullet : enemyBullets_)
		{
			bullet->Update(len, bulletSpeed, enemyObj);
		}

		break;

	case Phase::Leave:

		break;

	case Phase::Death:
		isDead = true;

		break;
	}

	enemyObj->Update();

}

void Enemy::Draw()
{
	enemyObj->Draw();

	for (EnemyBullet* bullet : enemyBullets_)
	{
		bullet->Draw();
	}
}

void Enemy::OnCollision()
{
	phase_ = Phase::Death;
}

void Enemy::SetParentCamera(Vector3 cameraWtf)
{
	enemyObj->wtf.position.z = cameraWtf.z + 45;
}