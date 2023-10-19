// ImguiManager.h
// Imguiの初期化処理などの管理

#pragma once
#include "WindowsApp.h"
#include "DirectXInitialize.h"

class ImguiManager
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="winApp"></param>
	/// <param name="dxInit"></param>
	void Initialize(WinApp* winApp, DirectXInitialize* dxInit);

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// Imguiフレーム開始
	/// </summary>
	void Begin();

	/// <summary>
	/// 描画前準備
	/// </summary>
	void End();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	HRESULT result;

	DirectXInitialize* dxInit_;

	//SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;

};