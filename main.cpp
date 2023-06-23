#include "WindowsApp.h"
#include "Input.h"
#include "DirectXInitialize.h"
#include "FPS.h"
#include "FBXLoader.h"
#include "GameScene.h"
#include "PostEffect.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{

#pragma region WindowsAPI初期化処理


	//ポインタ
	WinApp* winApp = nullptr;
	DirectXInitialize* dxInit = nullptr;
	FPS* fps = new FPS;
	Input* input = nullptr;
	GameScene* gameScene = nullptr;
	PostEffect* postEffect = nullptr;

	//windowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	//DirectXの初期化
	dxInit = new DirectXInitialize();
	dxInit->Initialize(winApp);

	//入力の初期化　
	input = new Input();
	input->Initialize(winApp);

	//FBX初期化
	FBXLoader::GetInstance()->Initialize(dxInit->GetDevice());

	//ポストエフェクト用テクスチャの読み込み
	//spriteCommon->LoadTexture(100, L"Resources/white1x1.png");

#pragma endregion

#pragma region DirectX初期化処理
	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxInit->GetDevice(), WinApp::window_width, WinApp::window_height);

	//パーティクル静的初期化
	ParticleManager::StaticInitialize(dxInit->GetDevice(), WinApp::window_width, WinApp::window_height);

#pragma endregion

#pragma region 描画初期化処理

	////////////////////////////
	//------音声読み込み--------//
	///////////////////////////

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize(dxInit, input);

	//FPS固定
	fps->SetFrameRate(60);

#pragma endregion
	//ゲームループ
	while (true)
	{
#pragma region ウィンドウメッセージ処理

		//アプリケーションが終わる時にmessageがWM_QUITになる
		if (winApp->ProcessMessage())
		{
			break;
		}
		if (input->PushKey(DIK_ESCAPE))
		{
			break;
		}

		fps->FpsControlBegin();

#pragma endregion

#pragma region DirectX毎フレーム処理
		/////////////////////////////////////////////////////
		//----------DireceX毎フレーム処理　ここから------------//
		/////////////////////////////////////////////////////

		//入力の更新
		input->Update();

		// ゲームシーンの毎フレーム処理
		gameScene->Update();

		//////////////////////////////////////////////
		//-------DireceX毎フレーム処理　ここまで--------//
		//////////////////////////////////////////////

#pragma endregion

#pragma region グラフィックスコマンド

		//4.描画コマンドここから
		dxInit->PreDraw();

		// ゲームシーンの描画
		//gameScene->Draw();

		// 描画終了
		dxInit->PostDraw();

		fps->FpsControlEnd();

		//4.描画コマンドここまで

#pragma endregion

#pragma region 画面入れ替え

#pragma endregion
	}
#pragma region  WindowsAPI後始末

	delete gameScene;

	//WindowsAPIの終了処理
	winApp->Finalize();

	//入力解放
	delete input;

	//WindowsAPI解放
	delete winApp;
	delete dxInit;

	//FBX解放
	FBXLoader::GetInstance()->Finalize();

	delete fps;

#pragma endregion
	return 0;
}