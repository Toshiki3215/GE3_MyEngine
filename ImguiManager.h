#pragma once
#include "WindowsApp.h"
#include "DirectXInitialize.h"

class ImguiManager
{
public:

	void Initialize(WinApp* winApp, DirectXInitialize* dxInit);

private:

	HRESULT result;

	//SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;

};