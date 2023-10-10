#include "TitleScene.h"

TitleScene::TitleScene()
{

}

TitleScene::~TitleScene()
{
	delete titleTex;
	delete selectBar;
}

void TitleScene::Initialize(DirectXInitialize* dxInit, Input* input)
{
	// nullptrチェック
	assert(input);

	input_ = input;

	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxInit);

	titleTex = new Sprite();
	titleTex->Initialize(spriteCommon);
	titleTex->SetPozition({ 0.0f,0.0f });
	titleTex->SetSize({ 1280.0f, 720.0f });

	selectBar = new Sprite();
	selectBar->Initialize(spriteCommon);
	selectBar->SetPozition({ selectBarPosX,selectBarPosY });
	selectBar->SetSize({ 1000.0f, 80.0f });

	text1 = new Sprite();
	text1->Initialize(spriteCommon);
	text1->SetPozition({ 32.0f,486.0f });
	text1->SetSize({ 256.0f, 64.0f });

	spriteCommon->LoadTexture(0, "Title.png");
	titleTex->SetTextureIndex(0);

	spriteCommon->LoadTexture(1, "selectBar.png");
	selectBar->SetTextureIndex(1);

	spriteCommon->LoadTexture(2, "titleText1.png");
	text1->SetTextureIndex(2);

}

void TitleScene::Update()
{
	if (input_->PushKey(DIK_UP) || input_->PushKey(DIK_W))
	{
		if (selectBarPosY <= posMin)
		{
			selectBarPosY = posMax;
		}
	}

	if (input_->PushKey(DIK_DOWN) || input_->PushKey(DIK_S))
	{
		if (selectBarPosY <= posMax)
		{
			selectBarPosY = posMin;
		}
	}

	selectBar->SetPozition({ selectBarPosX,selectBarPosY });

	if (input_->PushKey(DIK_SPACE) && selectBarPosY <= posMax)
	{
		startFlg = TRUE;
	}

}

void TitleScene::Draw()
{
	titleTex->Draw();
	selectBar->Draw();
	if (selectBarPosY <= posMax)
	{
		text1->Draw();
	}
}
