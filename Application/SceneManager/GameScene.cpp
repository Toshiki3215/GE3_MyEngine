// GameScene.cpp
// ゲームシーンを管理している

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

	//playerPos = { 0,0,0 };

	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);
	railCamera = new RailCamera(WinApp::window_width, WinApp::window_height);

	camWtf.Initialize();
	camWtf.position = camera->GetEye();

	railCamera->Initialize(camWtf);

	targetWtf.Initialize();
	//targetWtf.position = { playerPos.x,playerPos.y,targetDistance };
	targetWtf.position = { 0,0,targetDistance };

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
	//スプライト共通部分の初期化

	// ---------- パーティクル ---------- //

	particleManager = ParticleManager::Create();
	particleManager->LoadTexture("texture2.png");
	particleManager->Update();

	// ---------- 3Dオブジェクト ---------- //

	skydomeMD = Model::LoadFromOBJ("skydome");
	skydome = Object3d::Create();
	skydome->SetModel(skydomeMD);
	skydome->wtf.scale = (Vector3{ 2000, 2000, 2000 });

	targetMD = Model::LoadFromOBJ("obj");
	targetObj = Object3d::Create();
	targetObj->SetModel(targetMD);
	targetObj->wtf.position = { targetWtf.position };

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
	titleScene->Initialize(dxInit, input);

	//シーン遷移
	sceneTrans = new SceneTransition();
	sceneTrans->Initialize(dxInit, input);

	//自キャラの生成
	//プレイヤー
	player_ = new Player();
	player_->Initialize(dxInit, input);

	player_->SetParent(&camWtf);
	player_->SetPos(Vector3{ 0, 0, 15 });

	//エネミー
	enemy_ = new Enemy();
	enemy_->Initilize(Vector3{ -10, 10, 70 });
	enemy_->SetParent(&camWtf);

	/*enemy2_ = new Enemy();
	enemy2_->Initilize(Vector3{ 10, 0, 80 });
	enemy2_->SetParent(&camWtf);*/

}

void GameScene::Reset()
{
	camWtf.Initialize();
	Vector3 eyePos = { 0,20,0 };
	camera->SetEye(eyePos);
	camWtf.position = camera->GetEye();

	railCamera->Initialize(camWtf);

	targetWtf.Initialize();
	//targetWtf.position = { playerPos.x,playerPos.y,targetDistance };

	//自キャラの生成
	//プレイヤー
	player_->Initialize(dxInit, input);

	//player_->SetParent(&camWtf);
	player_->SetPos(Vector3{ 0, 0, 15 });

	enemy_->Initilize(Vector3{ -10, 10, 70 });

	//enemy2_->Initilize(Vector3{ 10, 0, 80 });
}

// ----- 毎フレーム処理 ----- //
void GameScene::Update()
{
	switch (scene)
	{
	case Scene::Title:

		titleScene->Update();

		rSpeed = 3.14f / 1000;
		skydome->wtf.rotation.y += rSpeed;

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
		startT++;
		if (startT >= 150)
		{
			isStart = TRUE;
			color += { 0, 0, 0, 0.1f };
			sceneTrans->UpdateColor(color);
			if (startT >= 300)
			{
				isStart2 = TRUE;
			}
		}
		GameStartEfe(isStart, isStart2);
		//CameraUpdate();

		// ---------- 3Dオブジェクト ---------- //
		skydome->Update();
		targetObj->wtf.position = camera->GetTarget();
		targetObj->Update();

		// ---------- FBX ---------- //

		player_->SetParentCamera(camera->GetEye());
		player_->Update();

		enemy_->SetParentCamera(camera->GetEye());
		enemy_->Update(player_->GetPos());
		//enemy2_->SetParentCamera(camera->GetEye());
		//enemy2_->Update(player_->GetPos());

		// ---------- パーティクル ---------- //
		if (player_->GetAlive() == FALSE)
		{
			isEffect = TRUE;
			GameScene::EffUpdate(player_->GetPos());
			changeT++;

			if (changeT >= 60)
			{
				scene = Scene::Gameover;
			}
		}
		/*isEffect = TRUE;
		GameScene::EffUpdate(player_->GetPos());*/
		//GameScene::EffUpdate(player_->GetWorldPosition());

		if (input->PushKey(DIK_R))
		{
			scene = Scene::Title;
		}

		break;

	case Scene::Clear:

		if (input->PushKey(DIK_SPACE))
		{
			scene = Scene::Title;
		}

		break;

	case Scene::Gameover:

		sceneTrans->endText();

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

		if (player_->GetAlive() == TRUE)
		{
			player_->Draw();
		}
		
		if (isStart == TRUE)
		{
			sceneTrans->Draw2();
		}
		//targetObj->Draw();

		//enemy_->Draw();
		//enemy2_->Draw();
		//sceneTrans->endDraw();

		// ---------- FBX ---------- //

		//fbxObject1->Draw(dxInit->GetCommandList());
		//fbxObject2->Draw(dxInit->GetCommandList());
		//playerFbx->Draw(dxInit->GetCommandList());

		break;

	case Scene::Clear:

		break;

	case Scene::Gameover:

		sceneTrans->endDraw();

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
		//GameScene::EffDraw2();

		// パーティクル描画後処理
		ParticleManager::PostDraw();

		break;

	case Scene::Clear:

		break;

	case Scene::Gameover:

		break;
	}
}

void GameScene::CameraMove()
{
	//左右移動
	if (input->PushKey(DIK_RIGHT))
	{
		cameraMoveSpeed = { 1,0,0 };
	}
	else if (input->PushKey(DIK_LEFT))
	{
		cameraMoveSpeed = { -1,0,0 };
	}
	else if (input->PushKey(DIK_UP))
	{
		cameraMoveSpeed = { 0,0,1 };
	}
	else if (input->PushKey(DIK_DOWN))
	{
		cameraMoveSpeed = { 0,0,-1 };
	}
	else if (input->PushKey(DIK_U))
	{
		cameraMoveSpeed = { 0,1,0 };
	}
	else if (input->PushKey(DIK_J))
	{
		cameraMoveSpeed = { 0,-1,0 };
	}
	else
	{
		cameraMoveSpeed = { 0,0,0 };
	}

	//注視点移動
	//camera->MoveTargetVector(cameraMoveSpeed);

	//視点移動 
	//camera->MoveEyeVector(cameraMoveSpeed);

	//注視点、視点両方移動
	camera->MoveVector(cameraMoveSpeed);

}

void GameScene::CameraUpdate()
{
	CameraMove();

	camera->Update();

}

void GameScene::GameStartEfe(bool isStart, bool isStart2)
{
	if (player_->GetAlive() == TRUE)
	{
		if (isStart == FALSE)
		{
			cameraMoveSpeed = { 0,-0.1f,0 };
			camera->MoveEyeVector(cameraMoveSpeed);
		}
		else if (isStart == TRUE)
		{
			if (isStart2 == TRUE)
			{
				//cameraMoveSpeed = { 0,0,0.5f };
				cameraMoveSpeed = { 0,0,0 };
				camera->MoveVector(cameraMoveSpeed);
			}
		}
	}

	camera->Update();
}

void GameScene::CheckAllCollisions()
{
	Vector3 posA, posB;

	//const std::list<PlayerBullet>& playerBullets = player_->GetBullets();

}

void GameScene::EffDraw()
{
	if (isEffect == 1)
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
	if (isEffect == 1)
	{
		// 3Dオブクジェクトの描画
		particleManager->Draw();
	}
	else
	{

	}
}

void GameScene::EffUpdate(Vector3 pos)
{
	//パーティクル範囲
	for (int i = 0; i < 20; i++)
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = 0.01f;
		//Vector3 pos = player_->GetWorldPosition();
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

		Vector3 rePos = { pos.x, pos.y + 2, pos.z };

		//追加
		particleManager->Add(100, rePos, vel, acc, 1.0f, 0.0f);

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
