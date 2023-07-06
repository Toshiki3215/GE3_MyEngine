#include "Player.h"

void Player::Initialize(Input* input)
{
	assert(input);

	this->input = input;

	playerPos = { 0,0,0 };
	playerRot = { 0,0,0 };
	playerScale = { 1,1,1 };

	moveTarget = Vector3(0, 0, 0);

	rotaTarget = Vector3(0, 0, 10);

	playerModel = Model::LoadFromOBJ("obj");
	playerObj = Object3d::Create();
	playerObj->SetModel(playerModel);
	playerObj->wtf.position = playerPos;
}

void Player::Update()
{
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

	//左右移動
	if (input->PushKey(DIK_D))
	{
		Vector3 speed = { 1,0,0 };

		//更新
		playerPos += speed;
	}
	if (input->PushKey(DIK_A))
	{
		Vector3 speed = { -1,0,0 };

		//更新
		playerPos += speed;
	}
	if (input->PushKey(DIK_S))
	{
		Vector3 speed = { 0,-1,0 };

		//更新
		playerPos += speed;
	}
	if (input->PushKey(DIK_W))
	{
		Vector3 speed = { 0,1,0 };

		//更新
		playerPos += speed;
	}

	playerObj->wtf.position = playerPos;
	playerObj->UpdateMat();
	playerObj->Update();

	//キャラクター攻撃処理
	Attack();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) 
	{
		bullet->Update();
	}

}

void Player::Draw()
{
	playerObj->Draw();

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) 
	{
		bullet->Draw();
	}
}

Vector3 Player::GetPlayerPos()
{
	return playerPos;
}

void Player::Attack() 
{
	if (input->PushKey(DIK_SPACE)) 
	{

		playerPos = playerObj->wtf.position;

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(kBulletSpeed * rotaTarget.x, 0, kBulletSpeed * rotaTarget.z);

		//弾生成し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initilize(playerPos, velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));

	}
}