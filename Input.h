#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

//“ü—Í
class Input
{
public: //ƒƒ“ƒo•Ï”

	//‰Šú‰»
	void Initialize(HRESULT result, WNDCLASSEX w);

	//XV
	void Update();

};
