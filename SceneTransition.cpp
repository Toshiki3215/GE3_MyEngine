#include "SceneTransition.h"

SceneTransition::SceneTransition()
{

}

SceneTransition::~SceneTransition()
{

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

	spriteCommon->LoadTexture(6, "transition1.png");
	transTex1->SetTextureIndex(6);

	spriteCommon->LoadTexture(7, "transition2.png");
	transTex2->SetTextureIndex(7);
}

void SceneTransition::UpdateStart()
{

	if (leftTransPos < defaultPos)
	{
		leftTransPos += sceneChangeSpead;
		if (leftTransPos >= defaultPos)
		{
			leftTransPos = defaultPos;
		}
	}

	if (rightTransPos > defaultPos)
	{
		rightTransPos -= sceneChangeSpead;
		if (rightTransPos <= defaultPos)
		{
			rightTransPos = defaultPos;
		}
	}

	if (leftTransPos >= defaultPos || rightTransPos <= defaultPos)
	{
		startTimer = 40.0f;
		isTrans = FALSE;
	}

	/*if (isTrans == FALSE)
	{
		startTimer--;
		if (startTimer <= 0.0f)
		{
			startTimer = 0.0f;
			if (leftTransPos >= -defaultPos2)
			{
				leftTransPos -= sceneChangeSpead;
				if (leftTransPos <= -defaultPos2)
				{
					leftTransPos = -defaultPos2;
				}
			}

			if (rightTransPos <= defaultPos2)
			{
				rightTransPos += sceneChangeSpead;
				if (rightTransPos >= defaultPos2)
				{
					rightTransPos = defaultPos2;
				}
			}
		}
	}*/

	transTex1->SetPozition({ leftTransPos,defaultPos });
	transTex2->SetPozition({ rightTransPos,defaultPos });

}

void SceneTransition::UpdateEnd()
{

	startTimer--;
	if (startTimer <= 0.0f)
	{
		startTimer = 0.0f;
		if (leftTransPos >= -defaultPos2)
		{
			leftTransPos -= sceneChangeSpead;
			if (leftTransPos <= -defaultPos2)
			{
				leftTransPos = -defaultPos2;
			}
		}

		if (rightTransPos <= defaultPos2)
		{
			rightTransPos += sceneChangeSpead;
			if (rightTransPos >= defaultPos2)
			{
				rightTransPos = defaultPos2;
			}
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
