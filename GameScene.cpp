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
	delete obj;
	delete obj2;
	delete tex1;
	delete tex2;
	delete floor;
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

	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	camWtf.Initialize();
	camWtf.position = { 0.0f, 3.0f, -8.0f };

	targetWtf.Initialize();
	targetWtf.position = { 0.0f,0.0f,targetDistance };

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
	/*tex1 = new Sprite();
	tex1->Initialize(spriteCommon);
	tex1->SetPozition({ 500,0 });
	tex1->SetSize({256.0f, 125.0f});

	tex2 = new Sprite();
	tex2->Initialize(spriteCommon);
	tex2->SetPozition({ 500,500 });
	tex2->SetSize({ 120.0f, 120.0f });*/

	//テクスチャ読込
	/*spriteCommon->LoadTexture(0, "bb.png");
	tex1->SetTextureIndex(0);

	spriteCommon->LoadTexture(1, "tex.png");
	tex2->SetTextureIndex(1);*/

	// ---------- パーティクル ---------- //
	/*particleManager = ParticleManager::Create();
	particleManager->LoadTexture("texture2.png");
	particleManager->Update();*/

	// ---------- 3Dオブジェクト ---------- //
	/*floorMD = Model::LoadFromOBJ("floor");
	floor = Object3d::Create();
	floor->SetModel(floorMD);
	floor->wtf.position = (Vector3{ 0, -10, 0 });*/

	skydomeMD = Model::LoadFromOBJ("skydome");
	skydome = Object3d::Create();
	skydome->SetModel(skydomeMD);
	skydome->wtf.scale = (Vector3{ 1000, 1000, 1000 });

	/*objMD = Model::LoadFromOBJ("obj");
	obj = Object3d::Create();
	obj->SetModel(objMD);
	obj->wtf.position = (Vector3{ 20, 20, 40 });

	obj2MD = Model::LoadFromOBJ("obj2");
	obj2 = Object3d::Create();
	obj2->SetModel(obj2MD);
	obj2->wtf.position = (Vector3{ 50, 0, 20 });*/

	//レベルデータの読み込み - jsonファイル読み込み
	//levelEData = LevelELoader::LoadFile("level_editer");
	levelEData = LevelELoader::LoadFile("TL_test");

	//レベルデータからオブジェクトを生成、配置
	for (auto& objData : levelEData->objects)
	{
		//ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(objData.fileName);
		if (it != models.end()) { model = it->second; }

		//モデルを指定して3Dオブジェクトを生成
		Object3d* newObject = Object3d::Create();
		if (objData.fileName == "ship")
		{
			model = Model::LoadFromOBJ("ship");		//levelEditer用のモデル
		}
		else if (objData.fileName == "floor")
		{
			model = Model::LoadFromOBJ("floor");		//levelEditer用のモデル
		}
		else 
		{
			model = Model::LoadFromOBJ("obj2");		//levelEditer用のモデル
		}
		newObject->SetModel(model);

		//座標
		newObject->wtf.position = objData.translation;

		//回転角
		newObject->wtf.rotation = Affin::radConvert3(objData.rotation);
		//newObject->wtf.rotation = Affin::degConvert3(objData.rotation);
		if (objData.fileName == "ship")
		{
			newObject->wtf.rotation.x = Affin::radConvert(objData.rotation.z);
			newObject->wtf.rotation.y = Affin::radConvert(objData.rotation.y);
			newObject->wtf.rotation.z = Affin::radConvert(objData.rotation.x);
		}
		else
		{
			newObject->wtf.rotation = Affin::radConvert3(objData.rotation);
		}

		//スケール
		newObject->wtf.scale = objData.scaling;

		//配列に登録
		objects.push_back(newObject);
	}

	// ---------- FBX ---------- //
	//モデル名を指定してファイル読み込み
	/*fbxModel1 = FBXLoader::GetInstance()->LoadModelFronmFile("cube");
	fbxModel2 = FBXLoader::GetInstance()->LoadModelFronmFile("boneTest");*/

	//FBXオブジェクト生成とモデルのセット
	/*fbxObject1 = new FBXObject;
	fbxObject1->Initialize();
	fbxObject1->SetModel(fbxModel1);

	fbxObject2 = new FBXObject;
	fbxObject2->Initialize();
	fbxObject2->SetModel(fbxModel2);
	fbxObject2->PlayAnimation();*/

	//ゲームフロー
	scene = Scene::Play;

}

void GameScene::Reset() 
{
	camWtf.Initialize();
	camWtf.position = { 0.0f, 3.0f, 0.0f };

	targetWtf.Initialize();
	targetWtf.position = { 0.0f,0.0f,targetDistance };

}

// ----- 毎フレーム処理 ----- //
void GameScene::Update() 
{
	switch (scene)
	{
	case Scene::Title:

		break;

	case Scene::Select:

		break;

	case Scene::Play:
		CamUpdate();

		// ---------- パーティクル ---------- //
		/*isEffFlag = 0;
		GameScene::EffUpdate();
		GameScene::EffUpdate2();*/
		
		// ---------- 3Dオブジェクト ---------- //
		//floor->Update();
		skydome->Update();
		/*obj->Update();
		obj2->Update();*/

		for (auto& object : objects) 
		{
			object->Update();
		}

		// ---------- FBX ---------- //
		/*fbxObject1->Update();
		fbxObject2->Update();*/

		break;

	case Scene::Clear:
		
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
	switch (scene)
	{
	case Scene::Title:

		break;

	case Scene::Select:

		break;

	case Scene::Play:
    
		// ---------- テクスチャ ---------- //
		/*tex1->Draw();
		tex2->Draw();*/

		// ---------- 3Dオブジェクト ---------- //
		//floor->Draw();
		skydome->Draw();
		/*obj->Draw();
		obj2->Draw();*/

		for (auto& object : objects) 
		{
			object->Draw();
		}
		
		// ---------- FBX ---------- //
		/*fbxObject1->Draw(dxInit->GetCommandList());
		fbxObject2->Draw(dxInit->GetCommandList());*/

		break;

	case Scene::Clear:

		break;

	case Scene::Gameover:

		break;
	}

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
		//ParticleManager::PreDraw(dxInit->GetCommandList());
		//GameScene::EffDraw();
		//GameScene::EffDraw2();

		//// パーティクル描画後処理
		//ParticleManager::PostDraw();

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
	targetWtf.position.z = cos(targetTheta) * targetDistance;
	targetWtf.position.y = sin(targetTheta) * targetDistance;
}

void GameScene::CamUpdate() 
{
	CamMove();
	CamRota();

	camWtf.UpdateMat();

	camera->SetEye(camWtf.position);

	targetWtf.UpdateMat();
	targetWtf.matWorld *= camWtf.matWorld;

	//y方向の制限
	if (targetWtf.matWorld.m[3][1] < 0) 
	{
		targetWtf.matWorld.m[3][1] = 0;
	}

	camera->SetTarget({ targetWtf.matWorld.m[3][0],targetWtf.matWorld.m[3][1] ,targetWtf.matWorld.m[3][2] });
	//camera->SetTarget({0,20,0 });

	camera->Update();
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