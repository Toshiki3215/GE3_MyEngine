#include "FPS.h"
#include "WindowsApp.h"
#include "DirectXInitialize.h"
#include "GameScene.h"
#include "FbxLoader.h"
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

	dxInit = new DirectXInitialize();
	dxInit->Initialize(winApp);

	MSG msg = {};

	//入力の初期化　
	input = new Input();
	input->Initialize(winApp);

	// ポストエフェクトの初期化
	postEffect = new PostEffect();
	postEffect->Initialize(dxInit, 1280, 720);

#pragma endregion

#pragma region DirectX初期化処理
	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxInit->GetDevice(), WinApp::window_width, WinApp::window_height);
	//パーティクル静的初期化
	ParticleManager::StaticInitialize(dxInit->GetDevice(), WinApp::window_width, WinApp::window_height);
	// FBX関連静的初期化
	FBXLoader::GetInstance()->Initialize(dxInit->GetDevice());

#pragma endregion

#pragma region 描画初期化処理

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize(dxInit, input);

	//FPS変えたいとき
	fps->SetFrameRate(60);

#pragma endregion
	//ゲームループ
	while (true) {
#pragma region ウィンドウメッセージ処理

		//window
		if (winApp->ProcessMessage()) 
		{
			//抜ける
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
		///////////////////////////////////////////////////

		//入力の更新
		input->Update();

		// ゲームシーンの毎フレーム処理
		gameScene->Update();

		//////////////////////////////////////////////
		//-------DireceX毎フレーム処理　ここまで--------//
		////////////////////////////////////////////

#pragma endregion

#pragma region グラフィックスコマンド

		postEffect->PreDrawScene(dxInit->GetCommandList());

		// ゲームシーンの描画
		gameScene->Draw();

		postEffect->PostDrawScene();

		//4.描画コマンドここから
		dxInit->PreDraw();

		postEffect->Draw(dxInit->GetCommandList());

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
	postEffect->Finalize();

	//入力開放
	delete input;

	//WindowsAPI開放
	delete winApp;
	delete dxInit;

	delete fps;

#pragma endregion
	return 0;
}