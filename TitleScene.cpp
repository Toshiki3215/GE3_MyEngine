#include "TitleScene.h"

TitleScene::TitleScene()
{

}

TitleScene::~TitleScene()
{
	delete titleTex;
}

void TitleScene::Initialize(DirectXInitialize* dxInit)
{
	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxInit);

	titleTex = new Sprite();
	titleTex->Initialize(spriteCommon);
	titleTex->SetPozition({ 0,0 });
	titleTex->SetSize({ 1280.0f, 720.0f });

	spriteCommon->LoadTexture(0, "Title.png");
	titleTex->SetTextureIndex(0);

	/*titlePlayerMD = Model::LoadFromOBJ("obj");
	titlePlayer = Object3d::Create();
	titlePlayer->SetModel(titlePlayerMD);*/
}

void TitleScene::Update()
{
	//titlePlayer->Update();
}

void TitleScene::Draw()
{
	titleTex->Draw();
	//titlePlayer->Draw();
}
