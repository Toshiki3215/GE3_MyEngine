#include "GameScene.h"
#include "FBXLoader.h"
#include "FBXObject.h"

/// <summary>
	/// コンストクラタ
	/// </summary>
GameScene::GameScene() 
{

}

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene() 
{
	delete spriteCommon;
	delete camera;
	delete skydome;
	delete fbxObject1;
	delete fbxModel1;
}

/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize(DirectXInitialize* dxInit, Input* input)
{
	// nullチェック
	assert(dxInit);
	assert(input);

	this->dxInit = dxInit;
	this->input = input;

	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxInit);

	playerPos = { 0,0,0 };

	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);
	railCamera = new RailCamera(WinApp::window_width, WinApp::window_height);

	camWtf.Initialize();
	camWtf.position = { 0.0f, 0.0f, -8.0f };

	railCamera->Initialize(camWtf);

	targetWtf.Initialize();
	targetWtf.position = { playerPos.x,playerPos.y,targetDistance };

	ParticleManager::SetCamera(camera);
	Object3d::SetCamera(camera);

	//デバイスをセット
	FBXObject::SetDevice(dxInit->GetDevice());

	//カメラをセット
	FBXObject::SetCamera(camera);

	//グラフィックスパイプライン生成
	FBXObject::CreateGraphicsPipeline();

	// ---------- テクスチャ ---------- //
	//テクスチャ生成

	//テクスチャ読込

	// ---------- パーティクル ---------- //

	particleManager = ParticleManager::Create();
	particleManager->LoadTexture("texture2.png");
	particleManager->Update();

	// ---------- 3Dオブジェクト ---------- //

	skydomeMD = Model::LoadFromOBJ("skydome");
	skydome = Object3d::Create();
	skydome->SetModel(skydomeMD);
	skydome->wtf.scale = (Vector3{ 2000, 2000, 2000 });

	// ---------- FBX ---------- //
	
	//モデル名を指定してファイル読み込み
	fbxModel1 = FBXLoader::GetInstance()->LoadModelFronmFile("cube");
	
	//FBXオブジェクト生成とモデルのセット
	fbxObject1 = new FBXObject;
	fbxObject1->Initialize();
	fbxObject1->SetModel(fbxModel1);

	/*fbxObject2 = new FBXObject;
	fbxObject2->Initialize();
	fbxObject2->SetModel(fbxModel2);
	fbxObject2->PlayAnimation();*/

	//シーン

	//ゲームフロー
	scene = Scene::Title;

	//タイトルシーン
	titleScene = new TitleScene();
	titleScene->Initialize(dxInit,input);

	//シーン遷移
	sceneTrans = new SceneTransition();
	sceneTrans->Initialize(dxInit, input);

	//自キャラの生成
	//プレイヤー
	player_ = new Player();
	player_->Initialize(dxInit, input);

	player_->SetParent(&camWtf);
	player_->SetPos(Vector3{ 0, 0, 20 });

	//エネミー
	enemy_ = new Enemy();
	enemy_->Initilize(Vector3{ -10, 10, 70 });
	enemy_->SetParent(&camWtf);

	enemy2_ = new Enemy();
	enemy2_->Initilize(Vector3{ 10, 0, 80 });
	enemy2_->SetParent(&camWtf);

}

void GameScene::Reset() 
{
	camWtf.Initialize();
	camWtf.position = { 0.0f, 0.0f, -8.0f };

	railCamera->Initialize(camWtf);

	targetWtf.Initialize();
	targetWtf.position = { playerPos.x,playerPos.y,targetDistance };

	//自キャラの生成
	//プレイヤー
	player_->Initialize(dxInit, input);

	//player_->SetParent(&camWtf);
	player_->SetPos(Vector3{ 0, 0, 20 });

	enemy_->Initilize(Vector3{ -10, 10, 70 });

	enemy2_->Initilize(Vector3{ 10, 0, 80 });
}

// ----- 毎フレーム処理 ----- //
void GameScene::Update() 
{
	switch (scene)
	{
	case Scene::Title:

		titleScene->Update();
		skydome->Update();

		if (titleScene->GetStartF() == TRUE)
		{
			sceneTrans->UpdateStart();
			if (sceneTrans->GetTransF() == FALSE)
			{
				scene = Scene::Play;
				Reset();
			}
		}

		break;

	case Scene::Select:

		break;

	case Scene::Play:
		sceneTrans->UpdateEnd();
		CamUpdate();
		CamUpdate2();

		// ---------- パーティクル ---------- //
		isEffFlag = 0;
		GameScene::EffUpdate();
		GameScene::EffUpdate2();

		// ---------- 3Dオブジェクト ---------- //
		skydome->Update();

		// ---------- FBX ---------- //
		
		player_->SetParentCamera(railCamera->GetEye());
		player_->Update();
		
		enemy_->SetParentCamera(railCamera->GetEye());
		enemy2_->SetParentCamera(railCamera->GetEye());
		enemy_->Update(player_->GetPos());
		enemy2_->Update(player_->GetPos());

		if (input->PushKey(DIK_L))
		{
			scene = Scene::Clear;
		}

		break;

	case Scene::Clear:

		if (input->PushKey(DIK_SPACE))
		{
			scene = Scene::Title;
		}

		break;

	case Scene::Gameover:
		
		break;
	}
}

// ----- 描画  ----- //
void GameScene::Draw()
{

	/// <summary>
	/// 3Dオブジェクトの描画
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// <summary>
	//3Dオブジェクト描画前処理
	Object3d::PreDraw(dxInit->GetCommandList());

	// ----- 3Dオブクジェクトの描画 ----- //
	skydome->Draw();

	switch (scene)
	{
	case Scene::Title:
		titleScene->Draw();
		break;

	case Scene::Select:

		break;

	case Scene::Play:
    
		// ---------- テクスチャ ---------- //

		// ---------- 3Dオブジェクト ---------- //

		player_->Draw();

		/*enemy_->Draw();
		enemy2_->Draw();*/
		
		// ---------- FBX ---------- //
		
		//fbxObject1->Draw(dxInit->GetCommandList());
		//fbxObject2->Draw(dxInit->GetCommandList());
		//playerFbx->Draw(dxInit->GetCommandList());

		break;

	case Scene::Clear:

		break;

	case Scene::Gameover:

		break;
	}

	sceneTrans->Draw();

	//3Dオブジェクト描画後処理
	Object3d::PostDraw();

	// ----- パーティクルの描画 ----- //
	switch (scene)
	{
	case Scene::Title:
		
		break;

	case Scene::Select:

		break;

	case Scene::Play:

		// パーティクル描画前処理
		ParticleManager::PreDraw(dxInit->GetCommandList());
		GameScene::EffDraw();
		GameScene::EffDraw2();

		// パーティクル描画後処理
		ParticleManager::PostDraw();

		break;

	case Scene::Clear:

		break;

	case Scene::Gameover:

		break;
	}
}

void GameScene::EffUpdate()
{
	//パーティクル範囲
	for (int i = 0; i < 20; i++) 
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = 0.01f;
		Vector3 pos{ 20, 20, 40 };
		pos.x += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_vel = 0.1f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_acc = 0.00001f;
		Vector3 acc{};
		acc.x = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
		acc.y = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;

		//追加
		particleManager->Add(60, pos, vel, acc, 1.0f, 0.0f);

		particleManager->Update();
	}

}

void GameScene::EffUpdate2()
{
	//パーティクル範囲
	for (int i = 0; i < 40; i++) 
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = 0.01f;
		Vector3 pos{ 50, 0, 20 };
		pos.x += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_vel = 0.1f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_acc = 0.00001f;
		Vector3 acc{};
		acc.x = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
		acc.y = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;

		//追加
		particleManager->Add(60, pos, vel, acc, 1.0f, 0.0f);

		particleManager->Update();
	}

}

void GameScene::EffDraw()
{
	if (isEffFlag == 1) 
	{
		// 3Dオブクジェクトの描画
		particleManager->Draw();
	}
	else 
	{

	}
}

void GameScene::EffDraw2()
{
	if (isEffFlag == 1)
	{
		// 3Dオブクジェクトの描画
		particleManager->Draw();
	}
	else 
	{

	}
}

void GameScene::CamMove() 
{
	//左右移動
	if (input->PushKey(DIK_RIGHT)) 
	{
		//カメラの移動
		Vector3 eyeVelocity = { 1.0,0,0 };

		//更新
		camWtf.position += eyeVelocity;
	}
	if (input->PushKey(DIK_LEFT))
	{
		//カメラの移動
		Vector3 eyeVelocity = { -1.0,0,0 };

		//更新
		camWtf.position += eyeVelocity;
	}

	//前後移動
	if (input->PushKey(DIK_UP))
	{
		//カメラの移動
		Vector3 eyeVelocity = { 0,0,1 };

		//更新
		camWtf.position += eyeVelocity;
	}
	if (input->PushKey(DIK_DOWN))
	{
		//カメラの移動
		Vector3 eyeVelocity = { 0,0,-1 };

		//更新
		camWtf.position += eyeVelocity;
	}

	//上下移動
	if (input->PushKey(DIK_U))
	{
		//カメラの移動
		Vector3 eyeVelocity = { 0,1,0 };

		//更新
		camWtf.position += eyeVelocity;
	}
	if (input->PushKey(DIK_J))
	{
		//カメラの移動
		Vector3 eyeVelocity = { 0,-1,0 };

		//更新
		camWtf.position += eyeVelocity;
	}

	Vector3 theta;

	if (input->PushKey(DIK_1))
	{
		theta.y = -5;

		//更新
		camWtf.rotation = theta;
	}

	//視点は一定の距離
	targetWtf.position.z = cosf(targetTheta) * targetDistance;
	targetWtf.position.y = sinf(targetTheta) * targetDistance;

}

void GameScene::CamMove2()
{
	//カメラの移動
	Vector3 eyeVelocity = { 0,0,-1.0 };

	if (input->PushKey(DIK_Q))
	{
		railCameraSwitch = true;
	}
	else if (input->PushKey(DIK_E))
	{
		railCameraSwitch = false;
	}

	if (railCameraSwitch == true)
	{
		//更新
		camWtf.position += eyeVelocity;

		targetWtf.position.z = cosf(targetTheta) * targetDistance + eyeVelocity.z;
		targetWtf.position.y = sinf(targetTheta) * targetDistance + eyeVelocity.y;
	}
	else 
	{
		targetWtf.position.z = cosf(targetTheta) * targetDistance;
		targetWtf.position.y = sinf(targetTheta) * targetDistance;
	}

	//視点は一定の距離
	/*targetWtf.position.z = cosf(targetTheta) * targetDistance;
	targetWtf.position.y = sinf(targetTheta) * targetDistance;*/

}

void GameScene::CamRota() 
{
	//視点移動

	//左右
	Vector3 theta;
	if (input->PushKey(DIK_A)) 
	{

		theta.y = -camRotaSpeed;
	}
	else if (input->PushKey(DIK_D)) 
	{
		theta.y = camRotaSpeed;
	}

	camWtf.rotation += theta;

	//上下
	if (input->PushKey(DIK_W)) 
	{
		targetTheta += camRotaSpeed;
	}
	else if (input->PushKey(DIK_S)) 
	{
		targetTheta += -camRotaSpeed;
	}
	
	//角度制限
	if (targetTheta < -PI / 5 * 2) 
	{
		targetTheta = -PI / 5 * 2;
	}
	else if (targetTheta > PI / 3)
	{ 
		targetTheta = PI / 3;
	}
	
	//視点は一定の距離
	targetWtf.position.z = cosf(targetTheta) * targetDistance;
	targetWtf.position.y = sinf(targetTheta) * targetDistance;
}

void GameScene::CamUpdate() 
{
	CamMove();
	//CamRota();

	camWtf.UpdateMat();

	camera->SetEye(camWtf.position);

	targetWtf.UpdateMat();
	targetWtf.matWorld *= camWtf.matWorld;

	//y方向の制限
	if (targetWtf.matWorld.m[3][1] < 0) 
	{
		targetWtf.matWorld.m[3][1] = 0;
	}

	//camera->SetTarget({ targetWtf.matWorld.m[3][0],targetWtf.matWorld.m[3][1] ,targetWtf.matWorld.m[3][2] });
	//camera->SetTarget({0,20,0 });
	camera->SetTarget(playerPos);

	camera->Update();
	railCamera->Update(camWtf);
}

void GameScene::CamUpdate2()
{
	CamMove2();
	//CamRota();

	camWtf.UpdateMat();

	railCamera->SetEye(camWtf.position);

	targetWtf.UpdateMat();
	targetWtf.matWorld *= camWtf.matWorld;

	//y方向の制限
	if (targetWtf.matWorld.m[3][1] < 0)
	{
		targetWtf.matWorld.m[3][1] = 0;
	}

	railCamera->SetTarget(playerPos);
	
	railCamera->Update(camWtf);
}

Vector3 GameScene::bVelocity(Vector3& velocity, Transform& worldTransform)
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

void GameScene::CheckAllCollisions()
{
	Vector3 posA, posB;

	//const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

}
