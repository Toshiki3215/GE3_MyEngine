#include "TitleScene.h"

TitleScene::TitleScene()
{

}

TitleScene::~TitleScene()
{
	delete titleTex;
	delete selectBar;
	delete selectBar2;
	delete selectBar3;
	delete text1;
	delete text2;
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

	selectBar2 = new Sprite();
	selectBar2->Initialize(spriteCommon);
	selectBar2->SetPozition({ selectBarPosX,selectBarPosY });
	selectBar2->SetSize({ 1000.0f, 80.0f });

	selectBar3 = new Sprite();
	selectBar3->Initialize(spriteCommon);
	selectBar3->SetPozition({ selectBarPosX,selectBarPosY });
	selectBar3->SetSize({ 1000.0f, 80.0f });

	text1 = new Sprite();
	text1->Initialize(spriteCommon);
	text1->SetPozition({ 32.0f,486.0f });
	text1->SetSize({ 256.0f, 64.0f });

	text2 = new Sprite();
	text2->Initialize(spriteCommon);
	text2->SetPozition({ 43.0f,614.0f });
	text2->SetSize({ 256.0f, 64.0f });

	spriteCommon->LoadTexture(0, "Title.png");
	titleTex->SetTextureIndex(0);

	spriteCommon->LoadTexture(1, "selectBar.png");
	selectBar->SetTextureIndex(1);

	spriteCommon->LoadTexture(2, "selectBar2.png");
	selectBar2->SetTextureIndex(2);

	spriteCommon->LoadTexture(3, "selectBar3.png");
	selectBar3->SetTextureIndex(3);

	spriteCommon->LoadTexture(4, "titleText1.png");
	text1->SetTextureIndex(4);

	spriteCommon->LoadTexture(5, "titleText2.png");
	text2->SetTextureIndex(5);

}

void TitleScene::Update()
{
	animeTimer++;
	if (animeTimer >= 40)
	{
		animeTimer = 0;
	}

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
	selectBar2->SetPozition({ selectBarPosX,selectBarPosY });
	selectBar3->SetPozition({ selectBarPosX,selectBarPosY });

	if (input_->PushKey(DIK_SPACE) && selectBarPosY <= posMax)
	{
		startFlg = TRUE;
	}

}

void TitleScene::Draw()
{
	titleTex->Draw();

	if (animeTimer <= 10)
	{
		selectBar3->Draw();
	}
	else if (animeTimer <= 20)
	{
		selectBar->Draw();
	}
	else if (animeTimer <= 30)
	{
		selectBar2->Draw();
	}
	else if (animeTimer <= 40)
	{
		selectBar->Draw();
	}
	
	if (selectBarPosY <= posMax)
	{
		text1->Draw();
	}
	if (selectBarPosY == posMin)
	{
		text2->Draw();
	}
}
