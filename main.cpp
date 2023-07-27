#include "WindowsApp.h"
#include "Input.h"
#include "DirectXInitialize.h"
#include "FPS.h"
#include "FBXLoader.h"
#include "GameScene.h"
#include "ImguiManager.h"
#include "Imgui/imgui.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{

#pragma region WindowsAPI初期化処理

	//ポインタ
	WinApp* winApp = nullptr;
	DirectXInitialize* dxInit = nullptr;
	FPS* fps = new FPS;
	Input* input = nullptr;
	GameScene* gameScene = nullptr;
	ImguiManager* imguiManager = nullptr;

	//windowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	//DirectXの初期化
	dxInit = new DirectXInitialize();
	dxInit->Initialize(winApp);

	//入力の初期化　
	input = new Input();
	input->Initialize(winApp);

	//Imguiの初期化
	imguiManager = new ImguiManager();
	imguiManager->Initialize(winApp,dxInit);

	//FBX初期化
	FBXLoader::GetInstance()->Initialize(dxInit->GetDevice());

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

		// Imgui受付開始
		imguiManager->Begin();

		// ゲームシーンの描画
		gameScene->Draw();

		ImGui::ShowDemoWindow();

		// Imgui受付終了
		imguiManager->End();

		// Imgui描画
		imguiManager->Draw();

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
	imguiManager->Finalize();

	//入力解放
	delete input;

	//WindowsAPI解放
	delete winApp;
	delete dxInit;

	delete imguiManager;

	//FBX解放
	FBXLoader::GetInstance()->Finalize();

	delete fps;

#pragma endregion
	return 0;
}