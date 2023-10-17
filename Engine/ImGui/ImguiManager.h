// ImguiManager.h
// Imguiの初期化処理などの管理

#pragma once
#include "WindowsApp.h"
#include "DirectXInitialize.h"

class ImguiManager
{
public:

	void Initialize(WinApp* winApp, DirectXInitialize* dxInit);

	//終了
	void Finalize();

	void Begin();

	void End();

	void Draw();

private:

	HRESULT result;

	DirectXInitialize* dxInit_;

	//SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;

};