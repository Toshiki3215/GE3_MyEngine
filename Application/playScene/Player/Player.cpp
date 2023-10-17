// Player.cpp
// Playerの処理を管理している

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
	playerMD2 = Model::LoadFromOBJ("obj2");

	playerObj = Object3d::Create();
	playerObj->SetModel(playerMD);
	playerObj->wtf.position = { 0.0f,0.0f,0.0f };

	//ビット
	bitModel1 = Model::LoadFromOBJ("obj2");
	bitObj1 = Object3d::Create();
	bitObj1->SetModel(bitModel1);
	bitObj1->wtf.scale = { 0.5f,0.5f,0.5f };
	bitObj1->wtf.position = playerObj->wtf.position;

	bitModel2 = Model::LoadFromOBJ("obj2");
	bitObj2 = Object3d::Create();
	bitObj2->SetModel(bitModel2);
	bitObj2->wtf.scale = { 0.5f,0.5f,0.5f };
	bitObj2->wtf.position = playerObj->wtf.position;

	//自機の弾
	shootModel_ = Model::LoadFromOBJ("obj4");
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
	bitObj1->Update();
	bitObj2->Update();

	retObj_->Update();
	enemylen = retObj_->wtf.position - shootObj_->wtf.position;
	enemylen.nomalize();

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });
	bitBullets1.remove_if([](std::unique_ptr<PlayerBullet>& bitBullet1) { return bitBullet1->IsDead(); });
	bitBullets2.remove_if([](std::unique_ptr<PlayerBullet>& bitBullet2) { return bitBullet2->IsDead(); });

	//プレイヤーの行動一覧
	PlayerAction();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update(enemylen, len, bulletSpeed, playerObj, retObj_);
	}
	for (std::unique_ptr<PlayerBullet>& bitBullet1 : bitBullets1)
	{
		bitBullet1->Update(enemylen, len, bulletSpeed, bitObj1, retObj_);
	}
	for (std::unique_ptr<PlayerBullet>& bitBullet2 : bitBullets2)
	{
		bitBullet2->Update(enemylen, len, bulletSpeed, bitObj2, retObj_);
	}
}

void Player::Draw()
{
	if (modeChange == false)
	{
		playerObj->SetModel(playerMD);
		playerObj->Draw();
	}
	else
	{
		playerObj->SetModel(playerMD2);
		playerObj->Draw();
		bitObj1->Draw();
		bitObj2->Draw();
	}

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw();
	}

	for (std::unique_ptr<PlayerBullet>& bitBullet1 : bitBullets1)
	{
		bitBullet1->Draw();
	}

	for (std::unique_ptr<PlayerBullet>& bitBullet2 : bitBullets2)
	{
		bitBullet2->Draw();
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
	/*if (modeChange == true)
	{
		if (input_->PushKey(DIK_SPACE))
		{
			rotaTime = 20;
		}

		if (rotaTime > 0)
		{
			rotaTime--;
			rSpeed = 3.14f / 10;
			playerObj->wtf.rotation.z -= rSpeed;
			if (rotaTime <= 0)
			{
				rSpeed = 0;
				playerObj->wtf.rotation.z = 0;
				rotaTime = 0;
			}
		}
	}
	else
	{

	}*/

	if (input_->PushKey(DIK_SPACE))
	{
		if (modeChange == false)
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
				newBullet->Initilize(playerObj, retObj_, bulletScale);

				//弾を登録する
				bullets_.push_back(std::move(newBullet));

				shotCool = true;
				coolTimer = 10;
			}
		}
		else if (modeChange == true)
		{
			//弾生成し、初期化
			std::unique_ptr<PlayerBullet> newBitBullet1 = std::make_unique<PlayerBullet>();
			std::unique_ptr<PlayerBullet> newBitBullet2 = std::make_unique<PlayerBullet>();
			newBitBullet1->Initilize(bitObj1, retObj_, bulletScale);
			newBitBullet2->Initilize(bitObj2, retObj_, bulletScale);

			//弾を登録する
			bullets_.push_back(std::move(newBitBullet1));
			bullets_.push_back(std::move(newBitBullet2));
		}
	}

	if (input_->PushKey(DIK_M))
	{
		modeChange = true;
	}
	else if (input_->PushKey(DIK_N))
	{
		modeChange = false;
	}

	if (modeChange == TRUE)
	{
		if (bitRotaTime > 0)
		{
			bitRotaTime--;
			rSpeed = 3.14f / 10;
			bitObj1->wtf.rotation.z -= rSpeed;
			bitObj2->wtf.rotation.z -= rSpeed;
			if (bitRotaTime <= 0)
			{
				bitRotaTime = 20;
			}
		}

		bitObj1->wtf.position = { playerObj->wtf.position.x + 2,playerObj->wtf.position.y,playerObj->wtf.position.z };
		bitObj2->wtf.position = { playerObj->wtf.position.x - 2,playerObj->wtf.position.y,playerObj->wtf.position.z };

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
	/*playerObj->wtf.position.x = cameraWtf.x;
	playerObj->wtf.position.y = cameraWtf.y; */
	playerObj->wtf.position.z = cameraWtf.z + 15;
	retObj_->wtf.position.z = cameraWtf.z + 25;
}

void Player::OnCollision()
{

}
