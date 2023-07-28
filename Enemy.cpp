#include "Enemy.h"
#include "Matrix4.h"
#include <cassert>

void Enemy::Initilize()
{
	//Ž©‹@
	enemyMD = Model::LoadFromOBJ("obj3");
	enemyObj = Object3d::Create();
	enemyObj->SetModel(enemyMD);
	enemyObj->wtf.position = { 0.0f,0.0f,+50.0f };
	enemyObj->wtf.scale = { 1.0f,1.0f,1.0f };
}

void Enemy::Update()
{
	switch (phase_) 
	{
	case Phase::Approch:
	default:
		enemyObj->wtf.position -= enemySpeed;

		if (enemyObj->wtf.position.z >= -10.0f)
		{
			phase_ = Phase::Death;
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
}

void Enemy::OnCollision()
{
	phase_ = Phase::Death;
}