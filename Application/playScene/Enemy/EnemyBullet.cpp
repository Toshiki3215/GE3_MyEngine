// EnemyBullet.cpp
// Enemyの攻撃(弾)を管理している

#include "EnemyBullet.h"

void EnemyBullet::Initilize(Object3d* enemyObj)
{
	//自機の弾
	bulletModel = Model::LoadFromOBJ("obj");
	bulletObj = Object3d::Create();
	bulletObj->SetModel(bulletModel);
	bulletObj->wtf.position = { enemyObj->wtf.position.x,enemyObj->wtf.position.y + 0.07f, enemyObj->wtf.position.z };
	bulletObj->wtf.scale = { 0.5f,0.5f,0.5f };

	//target = retObj->wtf.position - enemyObj->wtf.position;
	target = bulletObj->wtf.position;
	target.z = bulletObj->wtf.position.z + 10 - enemyObj->wtf.position.z;
	target.nomalize();

}

void EnemyBullet::Update(Vector3 len, float shootSpeed, Object3d* enemyObj)
{
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

	if (isDead_ == true)
	{
		bulletObj->wtf.position = { enemyObj->wtf.position.x,enemyObj->wtf.position.y + 0.07f, enemyObj->wtf.position.z };
	}
	else
	{
		//target.z = bulletObj->wtf.position.z + 10;

		bulletObj->wtf.position += len;
		//len = target;
		len *= shootSpeed;
	}

	bulletObj->Update();

}

void EnemyBullet::Draw()
{
	bulletObj->Draw();
}

Vector3 EnemyBullet::GetWorldPos()
{
	return bulletObj->wtf.position;
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}
