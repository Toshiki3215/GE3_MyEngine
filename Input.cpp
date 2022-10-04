#include "Input.h"

void Input::Initialize(HRESULT result , WNDCLASSEX w)
{
	//DirectInputの初期化
	IDirectInput8* directInput = nullptr;
	result = DirectInput8Create(w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	
	
	//キーボードデバイスの生成
	IDirectInputDevice8* keyboard = nullptr;
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	

	//入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard); //標準形式

	//入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard); //標準形式

}

void Input::Update()
{

}