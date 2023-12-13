// SceneTransition.cpp
// シーン遷移の管理

#include "SceneTransition.h"

SceneTransition::SceneTransition()
{

}

SceneTransition::~SceneTransition()
{
	delete transTex1;
	delete transTex2;
	delete UITex;
	delete gameOverTex;
	delete gameClearTex;
	delete playerHPTex;
}

void SceneTransition::Initialize(DirectXInitialize* dxInit, Input* input)
{
	// nullptrチェック
	assert(input);

	input_ = input;

	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxInit);

	transTex1 = new Sprite();
	transTex1->Initialize(spriteCommon);
	transTex1->SetPozition({ leftTransPos,defaultPos });
	transTex1->SetSize({ 1280.0f, 720.0f });

	transTex2 = new Sprite();
	transTex2->Initialize(spriteCommon);
	transTex2->SetPozition({ rightTransPos,defaultPos });
	transTex2->SetSize({ 1280.0f, 720.0f });

	UITex = new Sprite();
	UITex->Initialize(spriteCommon);
	UITex->SetPozition({ 0,0 });
	UITex->SetSize({ 1280.0f, 720.0f });

	gameOverTex = new Sprite();
	gameOverTex->Initialize(spriteCommon);
	gameOverTex->SetPozition({ 0,0 });
	gameOverTex->SetSize({ 1280.0f, 720.0f });

	gameClearTex = new Sprite();
	gameClearTex->Initialize(spriteCommon);
	gameClearTex->SetPozition({ 0,0 });
	gameClearTex->SetSize({ 1280.0f, 720.0f });

	playerHPTex = new Sprite();
	playerHPTex->Initialize(spriteCommon);
	playerHPTex->SetPozition(UIpos);
	playerHPTex->SetSize({ 720.0f, 72.0f });

	//テクスチャ読込
	spriteCommon->LoadTexture(6, "transition1.png");
	transTex1->SetTextureIndex(6);

	spriteCommon->LoadTexture(7, "transition2.png");
	transTex2->SetTextureIndex(7);

	spriteCommon->LoadTexture(8, "UI.png");
	UITex->SetTextureIndex(8);

	spriteCommon->LoadTexture(9, "gameover.png");
	gameOverTex->SetTextureIndex(9);

	spriteCommon->LoadTexture(11, "gameclear.png");
	gameClearTex->SetTextureIndex(11);

	spriteCommon->LoadTexture(12, "UI_HP.png");
	playerHPTex->SetTextureIndex(12);

}

void SceneTransition::UpdateStart()
{
	animeTimer++;

	leftTransPos += easing_->easeOut(animeTimer, defaultPos, defaultPos2, maxTime);
	if (leftTransPos >= defaultPos)
	{
		leftTransPos = defaultPos;
	}

	rightTransPos -= easing_->easeOut(animeTimer, defaultPos, defaultPos2, maxTime);
	if (rightTransPos <= defaultPos)
	{
		rightTransPos = defaultPos;
	}

	if (leftTransPos >= defaultPos && rightTransPos <= defaultPos)
	{
		waitTimer = 60.0f;
		animeTimer = 0;
		isTrans = FALSE;
	}

	transTex1->SetPozition({ leftTransPos,defaultPos });
	transTex2->SetPozition({ rightTransPos,defaultPos });

}

void SceneTransition::UpdateEnd()
{

	waitTimer--;
	if (waitTimer <= 0.0f)
	{
		animeTimer++;
		if (leftTransPos >= -defaultPos2)
		{
			leftTransPos -= easing_->easeOut(animeTimer, defaultPos, defaultPos2, maxTime);
			if (leftTransPos <= -defaultPos2)
			{
				leftTransPos = -defaultPos2;
			}
		}

		if (rightTransPos <= defaultPos2)
		{
			rightTransPos += easing_->easeOut(animeTimer, defaultPos, defaultPos2, maxTime);
			if (rightTransPos >= defaultPos2)
			{
				rightTransPos = defaultPos2;
			}
		}

		if (leftTransPos >= defaultPos2 && rightTransPos <= defaultPos2)
		{
			animeTimer = 0;
			waitTimer = 60.0f;
			isTrans = TRUE;
		}
	}

	transTex1->SetPozition({ leftTransPos,defaultPos });
	transTex2->SetPozition({ rightTransPos,defaultPos });
}

void SceneTransition::Draw()
{
	transTex1->Draw();
	transTex2->Draw();
}

void SceneTransition::Draw2()
{
	UITex->Draw();
	playerHPTex->Draw();
}

void SceneTransition::gameoverDraw()
{
	gameOverTex->Draw();
}

void SceneTransition::gameclearDraw()
{
	gameClearTex->Draw();
}

void SceneTransition::EndText()
{
	//animeTimer2++;

	//pos3 += easeOut(animeTimer2, -720, 0, 500);
	//pos3 += easing_->easeOut(animeTimer2, -720, 0, 500);
	//if (pos3 >= 0)
	//{
	//	pos3 = 0;
	//	//animeTimer = 0;
	//}

	//endTex->SetPozition({ 0,pos3 });

	gameOverTex->SetPozition({ 0,0 });
}

void SceneTransition::Reset()
{
	waitTimer = 60.0f;
	animeTimer = 0.0f;
	maxTime = 1200.0f;

	isTrans = TRUE;

	/*animeTimer2 = 0;
	pos1 = -720;
	pos2 = 0;
	pos3 = -720;*/

	leftTransPos = -defaultPos2;
	rightTransPos = defaultPos2;

	transTex1->SetPozition({ leftTransPos,defaultPos });
	transTex2->SetPozition({ rightTransPos,defaultPos });

}

void SceneTransition::UpdateUI(float hp)
{
	if (hp == 5)
	{
		UIpos.x = 0.0f;
	}
	else if (hp ==4)
	{
		UIpos.x = -90.0f;
	}
	else if (hp == 3)
	{
		UIpos.x = -180.0f;
	}
	else if (hp == 2)
	{
		UIpos.x = -270.0f;
	}
	else if (hp == 1)
	{
		UIpos.x = -360.0f;
	}
	else if (hp == 0)
	{
		UIpos.x = -450.0f;
	}

	playerHPTex->SetPozition({UIpos});
}
