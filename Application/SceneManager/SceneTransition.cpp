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
	//UITex->SetColor(Vector4{ 1,1,1,0.5f });

	endTex = new Sprite();
	endTex->Initialize(spriteCommon);
	endTex->SetPozition({ 0,pos3 });
	endTex->SetSize({ 1280.0f, 720.0f });

	//テクスチャ読込
	spriteCommon->LoadTexture(6, "transition1.png");
	transTex1->SetTextureIndex(6);

	spriteCommon->LoadTexture(7, "transition2.png");
	transTex2->SetTextureIndex(7);

	spriteCommon->LoadTexture(8, "UI.png");
	UITex->SetTextureIndex(8);

	spriteCommon->LoadTexture(9, "gameover.png");
	endTex->SetTextureIndex(9);
}

void SceneTransition::UpdateStart()
{
	animeTimer++;

	leftTransPos += easeOut(animeTimer, defaultPos, defaultPos2, maxTime);
	if (leftTransPos >= defaultPos)
	{
		leftTransPos = defaultPos;
	}

	rightTransPos -= easeOut(animeTimer, defaultPos, defaultPos2, maxTime);
	if (rightTransPos <= defaultPos)
	{
		rightTransPos = defaultPos;
	}

	if (leftTransPos >= defaultPos && rightTransPos <= defaultPos)
	{
		startTimer = 60.0f;
		animeTimer = 0;
		isTrans = FALSE;
	}

	transTex1->SetPozition({ leftTransPos,defaultPos });
	transTex2->SetPozition({ rightTransPos,defaultPos });

}

void SceneTransition::UpdateEnd()
{

	startTimer--;
	if (startTimer <= 0.0f)
	{
		animeTimer++;
		startTimer = 0.0f;
		if (leftTransPos >= -defaultPos2)
		{
			leftTransPos -= easeOut(animeTimer, defaultPos, defaultPos2, maxTime);
			if (leftTransPos <= -defaultPos2)
			{
				leftTransPos = -defaultPos2;
			}
		}

		if (rightTransPos <= defaultPos2)
		{
			rightTransPos += easeOut(animeTimer, defaultPos, defaultPos2, maxTime);
			if (rightTransPos >= defaultPos2)
			{
				rightTransPos = defaultPos2;
			}
		}
	}

	if (leftTransPos >= defaultPos2 && rightTransPos <= defaultPos2)
	{
		isTrans = TRUE;
	}

	transTex1->SetPozition({ leftTransPos,defaultPos });
	transTex2->SetPozition({ rightTransPos,defaultPos });
}

void SceneTransition::Draw()
{
	transTex1->Draw();
	transTex2->Draw();
}

void SceneTransition::UpdateColor(Vector4 color)
{
	UITex->SetColor(color);
}

void SceneTransition::Draw2()
{
	UITex->Draw();
}

void SceneTransition::endDraw()
{
	endTex->Draw();
}

void SceneTransition::endText()
{
	animeTimer2++;

	pos3 += easeOut(animeTimer2, -720, 0, 500);
	if (pos3 >= 0)
	{
		pos3 = 0;
		//animeTimer = 0;
	}

	//endTex->SetPozition({ 0,pos3 });
	endTex->SetPozition({ 0,0 });
}

float SceneTransition::easeIn(float time, float startPos, float endPos, float maxTime)
{
	float x = time / maxTime;
	float v = pow3(x);
	float z = endPos - startPos;
	float ret = z * v + startPos;

	return ret;
}

float SceneTransition::easeOut(float time, float startPos, float endPos, float maxTime)
{
	float x = time / maxTime;
	float v = 1 - pow3(1 - x);
	float z = endPos - startPos;
	float ret = z * v + startPos;

	return ret;
}

float SceneTransition::pow3(float x)
{
	return x * x * x;
}

float SceneTransition::pow5(float x)
{
	return x * x * x * x * x;
}
