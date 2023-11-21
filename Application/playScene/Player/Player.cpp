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
	delete bulletObj_;
	delete bulletModel_;
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
	bulletModel_ = Model::LoadFromOBJ("obj4");
	bulletObj_ = Object3d::Create();
	bulletObj_->SetModel(bulletModel_);
	bulletObj_->wtf.position = { playerObj->wtf.position.x,playerObj->wtf.position.y + 0.07f, playerObj->wtf.position.z };
	bulletObj_->wtf.scale = { 0.5f,0.5f,0.5f };

	//レティクル
	retModel_ = Model::LoadFromOBJ("obj2");
	retObj_ = Object3d::Create();
	retObj_->SetModel(retModel_);
	retObj_->wtf.scale = { 0.5f,0.5f,0.5f };
	retObj_->wtf.position = { -1.5f,1.0f,10.0f };

	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxInit);

	reticle = new Sprite();
	reticle->Initialize(spriteCommon);
	reticle->SetPozition({ retObj_->wtf.position.x,retObj_->wtf.position.y });
	reticle->SetSize({ 64.0f, 64.0f });

	//テクスチャ読込
	spriteCommon->LoadTexture(10, "reticle.png");
	reticle->SetTextureIndex(10);

}

void Player::Update()
{
	playerObj->Update();
	bitObj1->Update();
	bitObj2->Update();

	retObj_->Update();
	enemylen = retObj_->wtf.position - bulletObj_->wtf.position;
	enemylen.nomalize();

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {return bullet->IsDead(); });
	/*bitBullets1.remove_if([](std::unique_ptr<PlayerBullet>& bitBullet1) {return bitBullet1->IsDead(); });
	bitBullets2.remove_if([](std::unique_ptr<PlayerBullet>& bitBullet2) {return bitBullet2->IsDead(); });*/

	if (playerHp == 0)
	{
		isAlive = FALSE;
	}

	//プレイヤーの行動一覧
	PlayerAction();

	//自機のワールド座標から3Dレティクルのワールド座標を計算(レティクルの途中)
	{
		////自機から3Dレティクルへの距離
		//const float distancePlayerToReticle = 10.0f;

		////自機から3Dレティクルへのオフセット(Z+向き)
		//Vector3 offset = { 0,0,1.0f };

		////自機のワールド行列の回転を反映
		//offset = offset * playerObj->wtf.position

	}


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
		if (isBitDraw == TRUE)
		{
			bitObj1->Draw();
			bitObj2->Draw();
		}
	}

	if (isRetDraw == TRUE)
	{
		retObj_->Draw();

		for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
		{
			bullet->Draw();
		}
	}

	if (isBitDraw == TRUE)
	{
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
		{
			bullet->Draw();
		}
		/*for (std::unique_ptr<PlayerBullet>& bitBullet1 : bitBullets1)
		{
			bitBullet1->Draw();
		}

		for (std::unique_ptr<PlayerBullet>& bitBullet2 : bitBullets2)
		{
			bitBullet2->Draw();
		}*/
	}

	if (isRetDraw == TRUE)
	{
		reticle->Draw();
	}
}

void Player::PlayerAction()
{
	if (isUpdateStop == FALSE)
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

		if (input_->PushKey(DIK_M))
		{
			modeChange = true;
		}
		else if (input_->PushKey(DIK_N))
		{
			modeChange = false;
		}
	}

	//移動(レティクル)

	retObj_->wtf.position.x = playerObj->wtf.position.x;
	retObj_->wtf.position.y = playerObj->wtf.position.y;
	retObj_->wtf.position.z = playerObj->wtf.position.z + 10;

	//retObj_->wtf.matWorld.MakeInverse(retObj_->wtf.matWorld);

	/*if (input_->PushKey(DIK_T))
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
	}*/

	reticle->SetPozition({ retObj_->wtf.position.x,retObj_->wtf.position.y });
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
			bulletSpeed = 0.05f;

			if (--coolTimer <= 0)
			{
				shotCool = false;
			}

			if (shotCool == false)
			{
				//弾生成し、初期化
				std::unique_ptr<PlayerBullet> newBitBullet1 = std::make_unique<PlayerBullet>();
				std::unique_ptr<PlayerBullet> newBitBullet2 = std::make_unique<PlayerBullet>();
				newBitBullet1->Initilize(bitObj1, retObj_, bulletScale);
				newBitBullet2->Initilize(bitObj2, retObj_, bulletScale);

				//弾を登録する
				bullets_.push_back(std::move(newBitBullet1));
				bullets_.push_back(std::move(newBitBullet2));

				shotCool = true;
				coolTimer = 10;
			}
			////弾生成し、初期化
			//std::unique_ptr<PlayerBullet> newBitBullet1 = std::make_unique<PlayerBullet>();
			//std::unique_ptr<PlayerBullet> newBitBullet2 = std::make_unique<PlayerBullet>();
			//newBitBullet1->Initilize(bitObj1, retObj_, bulletScale);
			//newBitBullet2->Initilize(bitObj2, retObj_, bulletScale);

			////弾を登録する
			//bullets_.push_back(std::move(newBitBullet1));
			//bullets_.push_back(std::move(newBitBullet2));
		}
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

		bitObj1->wtf.position = { playerObj->wtf.position.x + bitSpace,playerObj->wtf.position.y,playerObj->wtf.position.z };
		bitObj2->wtf.position = { playerObj->wtf.position.x - bitSpace,playerObj->wtf.position.y,playerObj->wtf.position.z };

	}
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

	bulletObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	BulletWorldPos.x = bulletObj_->wtf.matWorld.m[3][0];
	BulletWorldPos.y = bulletObj_->wtf.matWorld.m[3][1];
	BulletWorldPos.z = bulletObj_->wtf.matWorld.m[3][2];

	return BulletWorldPos;
}

Vector3 Player::GetRetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 RetWorldPos;

	bulletObj_->wtf.UpdateMat();
	//ワールド行列の平行移動成分
	RetWorldPos.x = retObj_->wtf.matWorld.m[3][0];
	RetWorldPos.y = retObj_->wtf.matWorld.m[3][1];
	RetWorldPos.z = retObj_->wtf.matWorld.m[3][2];

	return RetWorldPos;
}

void Player::SetParentCamera(Vector3 cameraWtf)
{
	playerObj->wtf.position.z = cameraWtf.z + 15;
	//retObj_->wtf.position.z = cameraWtf.z + 25;
}

void Player::OnCollision()
{
	playerHp--;
	
}

void Player::PlayerModeChange()
{
	isUpdateStop = TRUE;
	isRetDraw = FALSE;

	if (bitSpace <= 0)
	{
		bitSpace = 0;
		isBitDraw = FALSE;
		clearAnimeTimer++;

		startPos = playerObj->wtf.position.z - 15;
		endPos = playerObj->wtf.position.z + 50;

		playerObj->wtf.position.z += easing->easeInBack(clearAnimeTimer, startPos, endPos, animeTimerEnd);
		playerObj->Update();

	}
	else 
	{
		bitSpace -= 0.01f;
	}

}
