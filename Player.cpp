#include "Player.h"

Player::Player() 
{

}

Player::~Player() 
{
	//FBXオブジェクト解放
	delete playerObj;
	delete playerMD;
	delete shootObj_;
	delete shootModel_;
	delete retObj_;
	delete retModel_;
}

void Player::Initialize(DirectXInitialize* dxInit, Input* input) 
{
	// nullptrチェック
	assert(dxInit);
	assert(input);

	this->dxInit = dxInit;
	input_ = input;

	//自機
	playerMD = Model::LoadFromOBJ("obj");
	playerObj = Object3d::Create();
	playerObj->SetModel(playerMD);
	playerObj->wtf.position = { 0.0f,-0.3f,0.0f };

	//自機の弾
	shootModel_ = Model::LoadFromOBJ("obj");
	shootObj_ = Object3d::Create();
	shootObj_->SetModel(shootModel_);
	shootObj_->wtf.position = { playerObj->wtf.position.x,playerObj->wtf.position.y + 0.07f, playerObj->wtf.position.z };
	shootObj_->wtf.scale = { 0.5f,0.5f,0.5f };

	//レティクル
	retModel_ = Model::LoadFromOBJ("obj2");
	retObj_ = Object3d::Create();
	retObj_->SetModel(retModel_);
	retObj_->wtf.scale = { 0.5f,0.5f,0.5f };
	retObj_->wtf.position = { -1.5f,1.0f,10.0f };
}

void Player::Update() 
{
	//playerObj->wtf.position = { 0.0f,0.0f,10.0f };
	playerObj->Update();
	retObj_->Update();
	enemylen = retObj_->wtf.position - shootObj_->wtf.position;
	enemylen.nomalize();

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

	//プレイヤーの行動一覧
	PlayerAction();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update(enemylen, len, bulletSpeed, playerObj,retObj_);
	}
}

void Player::Draw() 
{
	playerObj->Draw();

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) 
	{
		bullet->Draw();
	}

	retObj_->Draw();
}

void Player::PlayerAction()
{
	//移動(自機)
	if (input_->PushKey(DIK_W)) 
	{
		playerObj->wtf.position.y += 0.1f;
	}
	else if (input_->PushKey(DIK_S)) 
	{
		playerObj->wtf.position.y -= 0.1f;
	}
	else if (input_->PushKey(DIK_A)) 
	{
		playerObj->wtf.position.x -= 0.1f;
	}
	else if (input_->PushKey(DIK_D)) 
	{
		playerObj->wtf.position.x += 0.1f;
	}
	//移動(レティクル)
	if (input_->PushKey(DIK_T)) 
	{
		retObj_->wtf.position.y += 0.08f;
	}
	else if (input_->PushKey(DIK_G)) 
	{
		retObj_->wtf.position.y -= 0.08f;
	}
	else if (input_->PushKey(DIK_F)) 
	{
		retObj_->wtf.position.x -= 0.08f;
	}
	else if (input_->PushKey(DIK_H)) 
	{
		retObj_->wtf.position.x += 0.08f;
	}
	//移動制限(自機とレティクル)
	/*if (playerObj->wtf.position.x >= 0.6f) 
	{
		playerObj->wtf.position.x = 0.6f;
	}
	else if (playerObj->wtf.position.x <= -0.6f) 
	{
		playerObj->wtf.position.x = -0.6f;
	}
	else if (playerObj->wtf.position.y >= 0.19f) 
	{
		playerObj->wtf.position.y = 0.19f;
	}
	else if (playerObj->wtf.position.y <= -0.35f) 
	{
		playerObj->wtf.position.y = -0.35f;
	}*/

	//弾発射
	if (input_->PushKey(DIK_SPACE)) 
	{
		bulletSpeed = 0.05f;

		if (--coolTimer <= 0)
		{
			shotCool = false;
		}

		if (shotCool == false)
		{
			//弾生成し、初期化
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Initilize(playerObj, retObj_);

			//弾を登録する
			bullets_.push_back(std::move(newBullet));

			shotCool = true;
			coolTimer = 10;
		}
	}
}

Vector3 Player::bVelocity(Vector3& velocity, Transform& worldTransform)
{
	Vector3 result = { 0,0,0 };

	//内積
	result.z = velocity.x * worldTransform.matWorld.m[0][2] +
		velocity.y * worldTransform.matWorld.m[1][2] +
		velocity.z * worldTransform.matWorld.m[2][2];

	result.x = velocity.x * worldTransform.matWorld.m[0][0] +
		velocity.y * worldTransform.matWorld.m[1][0] +
		velocity.z * worldTransform.matWorld.m[2][0];

	result.y = velocity.x * worldTransform.matWorld.m[0][1] +
		velocity.y * worldTransform.matWorld.m[1][1] +
		velocity.z * worldTransform.matWorld.m[2][1];

	return result;
}

Vector3 Player::GetWorldPosition() 
{
	//ワールド座標を入れる変数
	Vector3 worldPos;

	playerObj->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	worldPos.x = playerObj->wtf.matWorld.m[3][0];
	worldPos.y = playerObj->wtf.matWorld.m[3][1];
	worldPos.z = playerObj->wtf.matWorld.m[3][2];

	return worldPos;
}

Vector3 Player::GetBulletWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 BulletWorldPos;

	shootObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	BulletWorldPos.x = shootObj_->wtf.matWorld.m[3][0];
	BulletWorldPos.y = shootObj_->wtf.matWorld.m[3][1];
	BulletWorldPos.z = shootObj_->wtf.matWorld.m[3][2];

	return BulletWorldPos;
}

Vector3 Player::GetRetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 RetWorldPos;

	shootObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	RetWorldPos.x = retObj_->wtf.matWorld.m[3][0];
	RetWorldPos.y = retObj_->wtf.matWorld.m[3][1];
	RetWorldPos.z = retObj_->wtf.matWorld.m[3][2];

	return RetWorldPos;
}

void Player::SetParentCamera(Vector3 cameraWtf)
{ 
	playerObj->wtf.position.x = cameraWtf.x; 
	playerObj->wtf.position.y = cameraWtf.y; 
	playerObj->wtf.position.z = cameraWtf.z + 10; 
}

void Player::OnCollision()
{

}
