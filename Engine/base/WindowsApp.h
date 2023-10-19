// WindowsApp.h
// WindowsAPIを管理してる

#pragma once
#include <Windows.h>

class WinApp
{
public:

	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd"></param>
	/// <param name="msg"></param>
	/// <param name="wparam"></param>
	/// <param name="lparam"></param>
	/// <returns></returns>
	static LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//ゲッター(HWND)
	HWND GetHwnd() const { return hwnd; }

	//ゲッター(HINSTANCE)
	HINSTANCE GetHInstance() const { return w.hInstance; }

	//プロセスメッセージ
	bool ProcessMessage();

	// ウィンドウ横幅
	static const int window_width = 1280;

	// ウィンドウ縦幅
	static const int window_height = 720;

public: //メンバ関数
	//初期化
	void Initialize();

	//更新
	void Update();

	//終了
	void Finalize();
private:
	//ウィンドウハンドル
	HWND hwnd = nullptr;

	//ウィンドウクラスの設定
	WNDCLASSEX w{};

};