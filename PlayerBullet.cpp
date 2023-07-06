#include "PlayerBullet.h"

void PlayerBullet::Initilize(const Vector3& position, const Vector3& velocity)
{
	bulletPos = position;
	bulletRot = { 0,0,0 };
	bulletScale = { 1,1,1 };
	
	bulletModel = Model::LoadFromOBJ("obj");
	bulletObj = Object3d::Create();
	bulletObj->SetModel(bulletModel);
	bulletObj->wtf.position = bulletPos;
	bulletObj->wtf.scale = bulletScale;

	//ˆø”‚ÅŽó‚¯Žæ‚Á‚½‘¬“x‚ðƒƒ“ƒo•Ï”‚É‘ã“ü
	velocity_ = velocity;

}

void PlayerBullet::Update()
{
	bulletPos += velocity_;
	bulletObj->wtf.position = bulletPos;

	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

}

void PlayerBullet::Draw()
{
	bulletObj->wtf.position = bulletPos;
	bulletObj->Draw();
}

Vector3 PlayerBullet::GetWorldPos()
{
	return bulletPos;
}
