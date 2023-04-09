#include "GameScene.h"


/// <summary>
	/// コンストクラタ
	/// </summary>
GameScene::GameScene() {
}

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene() {
	delete spriteCommon;
	delete camera;
	delete obj;
	delete obj2;
	delete tex1;
	delete tex2;
	delete floor;
	//delete skydome;
}

/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize(DirectXCommon* dxCommon, Input* input) {
	// nullチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	this->input = input;

	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	camWtf.Initialize();
	camWtf.position = { 0.0f, 3.0f, -8.0f };

	targetWtf.Initialize();
	targetWtf.position = { 0.0f,0.0f,targetDistance };

	ParticleManager::SetCamera(camera);
	Object3d::SetCamera(camera);

	floorMD = Model::LoadFromOBJ("floor");
	floor = Object3d::Create();
	floor->SetModel(floorMD);
	floor->wtf.position = (Vector3{ 0, -10, 0 });

	//skydomeMD = Model::LoadFromOBJ("skydome");
	//skydome = Object3d::Create();
	//skydome->SetModel(skydomeMD);
	//skydome->wtf.scale = (Vector3{ 1000, 1000, 1000 });

	objMD = Model::LoadFromOBJ("obj");
	obj = Object3d::Create();
	obj->SetModel(objMD);
	obj->wtf.position = (Vector3{ 20, 20, 40 });

	obj2MD = Model::LoadFromOBJ("obj2");
	obj2 = Object3d::Create();
	obj2->SetModel(obj2MD);
	obj2->wtf.position = (Vector3{ 50, 0, 20 });

	tex1 = new Sprite();
	tex1->Initialize(spriteCommon);
	tex1->SetPozition({ 500,0 });
	tex1->SetSize({256.0f, 125.0f});

	tex2 = new Sprite();
	tex2->Initialize(spriteCommon);
	tex2->SetPozition({ 500,500 });
	tex2->SetSize({ 120.0f, 120.0f });

	//ゲームフロー
	scene = Scene::Play;
	//stage = 0;

	spriteCommon->LoadTexture(0, "bb.png");
	tex1->SetTextureIndex(0);

	spriteCommon->LoadTexture(1, "tex.png");
	tex2->SetTextureIndex(1);

}

void GameScene::Reset() {
	camWtf.Initialize();
	camWtf.position = { 0.0f, 3.0f, -8.0f };

	targetWtf.Initialize();
	targetWtf.position = { 0.0f,0.0f,targetDistance };

}

// 毎フレーム処理
void GameScene::Update() {
	switch (scene)
	{
	case Scene::Title:

		break;

	case Scene::Select:

		break;

	case Scene::Play:
		CamUpdate();
		
    floor->Update();
	obj->Update();
	obj2->Update();
    //skydome->Update();

		break;

	case Scene::Clear:
		
		break;

	case Scene::Gameover:
		
		break;
	}
}

// 描画
void GameScene::Draw() 
{

	/// <summary>
	/// 3Dオブジェクトの描画
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// <summary>
	//3Dオブジェクト描画前処理
	Object3d::PreDraw(dxCommon->GetCommandList());
	//// 3Dオブクジェクトの描画
	switch (scene)
	{
	case Scene::Title:

		break;

	case Scene::Select:

		break;

	case Scene::Play:

		/*player_->Draw();
		enemyManager_->Draw();*/
    
    floor->Draw();
    obj->Draw();
    obj2->Draw();
    //skydome->Draw();
	tex1->Draw();
	tex2->Draw();

		break;

	case Scene::Clear:

		break;

	case Scene::Gameover:

		break;
	}

	//3Dオブジェクト描画後処理
	Object3d::PostDraw();

	// パーティクルの描画
	switch (scene)
	{
	case Scene::Title:
		
		break;

	case Scene::Select:

		break;

	case Scene::Play:

		// パーティクル描画前処理
		ParticleManager::PreDraw(dxCommon->GetCommandList());
		//player_->EffDraw();

		// パーティクル描画後処理
		ParticleManager::PostDraw();

		break;

	case Scene::Clear:

		break;

	case Scene::Gameover:

		break;
	}
}


void GameScene::CamMove() 
{
	if (input->PushKey(DIK_A)) 
	{
		//カメラの移動
		Vector3 eyeVelocity = { 0,0,0 };

		//更新
		camWtf.position += eyeVelocity;
	}
}

void GameScene::CamRota() {
	//視点移動

	//左右
	Vector3 theta;
	if (input->PushKey(DIK_A)) {

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