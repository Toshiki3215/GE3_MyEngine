#include "GameScene.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() 
{
	
	//FBXオブジェクト解放
	delete fbxObject1;
	delete fbxModel1;

}

void GameScene::Initialize(DirectXInitialize* dxInit, Input* input) {
	// nullptrチェック
	assert(dxInit);
	assert(input);

	this->dxInit = dxInit;
	this->input = input;

	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxInit);

	// カメラ生成
	camera = new Camera(1280, 720);
	FBXObject::SetCamera(camera);

	fbxModel1 = FBXLoader::GetInstance()->LoadModelFromFile("boneTest");

	// デバイスをセット
	FBXObject::SetDevice(dxInit->GetDevice());

	// グラフィックスパイプライン生成
	FBXObject::CreateGraphicsPipeline();


	fbxObject1 = new FBXObject;
	fbxObject1->Initialize();
	fbxObject1->SetModel(fbxModel1);
	fbxObject1->SetScale({ 5.0,5.0,5.0 });
	fbxObject1->SetPosition({ 0,-15,40 });
	fbxObject1->PlayAnimation();
	
}


void GameScene::Update() 
{
	
	camera->Update();
	
	fbxObject1->Update();

}

void GameScene::Draw() 
{
	
	fbxObject1->Draw(dxInit->GetCommandList());

	//3Dオブジェクト描画後処理
	Object3d::PostDraw();

}