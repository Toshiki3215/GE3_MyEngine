#include "PlayerBullet.h"

void PlayerBullet::Initilize(Object3d* playerObj, Object3d* retObj)
{
	//Ž©‹@‚Ì’e
	bulletModel = Model::LoadFromOBJ("obj");
	bulletObj = Object3d::Create();
	bulletObj->SetModel(bulletModel);
	bulletObj->wtf.position = { playerObj->wtf.position.x,playerObj->wtf.position.y + 0.07f, playerObj->wtf.position.z };
	bulletObj->wtf.scale = { 0.5f,0.5f,0.5f };

	target = retObj->wtf.position - playerObj->wtf.position;
	target.nomalize();

}

void PlayerBullet::Update(Vector3 enemylen, Vector3 len, float shootSpeed, Object3d* playerObj, Object3d* retObj)
{

	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

	if (isDead_ == true)
	{
		bulletObj->wtf.position = { playerObj->wtf.position.x,playerObj->wtf.position.y + 0.07f, playerObj->wtf.position.z };
	}
	else
	{
		bulletObj->wtf.position += target;
		//bulletObj->wtf.position += len;
		len = target;
		len *= shootSpeed;
	}

	if (bulletObj->wtf.position.z >= retObj->wtf.position.z)
	{
		//isDead_ = true;
	}

	bulletObj->Update();

	/*if (bulletObj->wtf.position.z >= retObj->wtf.position.z)
	{
		isDead_ = false;
	}*/

}

void PlayerBullet::Draw()
{
	bulletObj->Draw();
}

Vector3 PlayerBullet::GetWorldPos()
{
	return bulletObj->wtf.position;
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}
