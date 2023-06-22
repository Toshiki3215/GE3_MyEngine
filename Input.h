#pragma once
#include <windows.h>
#include <wrl.h>
#include <dinput.h>
#include "WindowsApp.h"

class Input
{
public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // メンバ関数

	// 初期化
	void Initialize(WinApp* winApp);

	// 更新
	void Update();

	bool PushKey(BYTE keyNumber);

	//トリガー
	bool TriggerKey(BYTE keyNumber);

	//離したか
	bool ReleaseKey(BYTE keyNumber);

private: // メンバ変数

	// キーボードのデバイス
	ComPtr<IDirectInputDevice8> keyboard;

	// DirectInputのインスタンス
	ComPtr<IDirectInput8> directInput;

	// 全キーの状態
	BYTE key[256] = {};

	// 前回の全キーの状態
	BYTE keyPre[256] = {};

	//windwsAPI
	WinApp* winApp_ = nullptr;
};