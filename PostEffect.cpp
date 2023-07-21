#include "PostEffect.h"
#include <d3dx12.h>
#include "WindowsApp.h"

using namespace DirectX;

//静的メンバ変数の実体					  Red  Green Blue Alpha
const float PostEffect::clearColor[4] = { 0.25f,0.5f,0.1f,0.0f };	// 緑っぽい色

PostEffect::PostEffect()
	:Sprite(
		100,
		{0,0},
		{500.0f,500.0f},
		{1,1,1,1},
		{0.0f,0.0f},
		false,
		false)
{
}

void PostEffect::Initialize(SpriteCommon* spritecommon_)
{
	HRESULT result;

	spritecomon = spritecommon_;

	//基底クラスとしての初期化
	Sprite::Initialize(spritecomon);

	CreateTexBuff();

	{
		////テクスチャリソース設定
		//CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		//	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		//	WinApp::window_width,
		//	(UINT)WinApp::window_height,
		//	1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
		//);

		////テクスチャバッファの生成
		//result = spritecomon->GetDxInitialize()->GetDevice()->CreateCommittedResource(
		//	&heapProp,
		//	D3D12_HEAP_FLAG_NONE,
		//	&texresDesc,
		//	D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		//	nullptr,
		//	IID_PPV_ARGS(&texBuff)
		//);

		//assert(SUCCEEDED(result));

		//{//テクスチャを赤クリア
		//	//画素数(1280 × 720 = 921600ピクセル)
		//	const UINT pixelCount = WinApp::window_width * WinApp::window_height;

		//	//画像1行分のデータサイズ
		//	const UINT rowPitch = sizeof(UINT) * WinApp::window_width;

		//	//画像全体のデータサイズ
		//	const UINT depthPitch = rowPitch * WinApp::window_height;

		//	//画像イメージ
		//	UINT* img = new UINT[pixelCount];
		//	for (int i = 0; i < pixelCount; i++)
		//	{
		//		img[i] = 0xff0000ff;	//R：ff,G：00,B：00,A：ff
		//	}

		//	//テクスチャバッファにデータ転送
		//	result = texBuff->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);

		//	assert(SUCCEEDED(result));
		//	delete[] img;
		//}
	}

	// ----- SRV ----- //
	CreateSRV();

	{
		////SRV用デスクリプタヒープ設定
		//D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
		//srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		//srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		//srvDescHeapDesc.NumDescriptors = 1;

		////SRV用デスクリプタヒープを生成
		//result = spritecomon->GetDxInitialize()->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
		//assert(SUCCEEDED(result));

		////SRV設定
		//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//設定構造体
		//srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		//srvDesc.Texture2D.MipLevels = 1;

		////デスクリプタヒープにSRV作成
		//spritecomon->GetDxInitialize()->GetDevice()->CreateShaderResourceView(
		//	texBuff.Get(),	//ビューと関連付けるバッファ
		//	&srvDesc,
		//	descHeapSRV->GetCPUDescriptorHandleForHeapStart()
		//);
	}

	// ----- RTV ----- //
	CreateRTV();

	{
		////RTV用デスクリプタヒープ設定
		//D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
		//rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		//rtvDescHeapDesc.NumDescriptors = 1;

		////RTV用デスクリプタヒープを生成
		//result = spritecomon->GetDxInitialize()->GetDevice()->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
		//assert(SUCCEEDED(result));

		////レンダーターゲットビューの設定
		//D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};

		////シェーダーの計算結果をSRGBに変換して書き込む
		//renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		//renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		////デスクリプタヒープにRTV作成
		//spritecomon->GetDxInitialize()->GetDevice()->CreateRenderTargetView(
		//	texBuff.Get(),
		//	&renderTargetViewDesc,
		//	descHeapRTV->GetCPUDescriptorHandleForHeapStart()
		//);
	}

	// ----- 深度バッファ ----- //
	CreateBuff();

	{
		////深度バッファリソース設定
		//CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		//	DXGI_FORMAT_D32_FLOAT,
		//	WinApp::window_width,
		//	WinApp::window_height,
		//	1, 0,
		//	1, 0,
		//	D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		//);

		////深度バッファの生成
		//result = spritecomon->GetDxInitialize()->GetDevice()->CreateCommittedResource(
		//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		//	D3D12_HEAP_FLAG_NONE,
		//	&depthResDesc,
		//	D3D12_RESOURCE_STATE_DEPTH_WRITE,
		//	&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		//	IID_PPV_ARGS(&depthBuff)
		//);
		//assert(SUCCEEDED(result));
	}

	// ----- DSV ----- //
	CreateDSV();

	{
		////DSV用デスクリプタヒープ設定
		//D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
		//DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		//DescHeapDesc.NumDescriptors = 1;

		////DSV用デスクリプタヒープを作成
		//result = spritecomon->GetDxInitialize()->GetDevice()->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
		//assert(SUCCEEDED(result));

		////デスクリプタヒープにDSV作成
		//D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		//dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		//dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		//spritecomon->GetDxInitialize()->GetDevice()->CreateDepthStencilView(
		//	depthBuff.Get(),
		//	&dsvDesc,
		//	descHeapDSV->GetCPUDescriptorHandleForHeapStart()
		//);
	}

}

void PostEffect::CreateTexBuff()
{
	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WinApp::window_width,
		(UINT)WinApp::window_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//テクスチャバッファの生成
	result = spritecomon->GetDxInitialize()->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,
		IID_PPV_ARGS(&texBuff)
	);

	assert(SUCCEEDED(result));

	{//テクスチャを赤クリア
		//画素数(1280 × 720 = 921600ピクセル)
		const UINT pixelCount = WinApp::window_width * WinApp::window_height;

		//画像1行分のデータサイズ
		const UINT rowPitch = sizeof(UINT) * WinApp::window_width;

		//画像全体のデータサイズ
		const UINT depthPitch = rowPitch * WinApp::window_height;

		//画像イメージ
		UINT* img = new UINT[pixelCount];
		for (int i = 0; i < pixelCount; i++)
		{
			img[i] = 0xff0000ff;	//R：ff,G：00,B：00,A：ff
		}

		//テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);

		assert(SUCCEEDED(result));
		delete[] img;
	}
}

void PostEffect::CreateSRV()
{
	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;

	//SRV用デスクリプタヒープを生成
	result = spritecomon->GetDxInitialize()->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//設定構造体
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//デスクリプタヒープにSRV作成
	spritecomon->GetDxInitialize()->GetDevice()->CreateShaderResourceView(
		texBuff.Get(),	//ビューと関連付けるバッファ
		&srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart()
	);
}

void PostEffect::CreateRTV()
{
	//RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;

	//RTV用デスクリプタヒープを生成
	result = spritecomon->GetDxInitialize()->GetDevice()->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));

	//レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};

	//シェーダーの計算結果をSRGBに変換して書き込む
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	//デスクリプタヒープにRTV作成
	spritecomon->GetDxInitialize()->GetDevice()->CreateRenderTargetView(
		texBuff.Get(),
		&renderTargetViewDesc,
		descHeapRTV->GetCPUDescriptorHandleForHeapStart()
	);
}

void PostEffect::CreateBuff()
{
	//深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		WinApp::window_width,
		WinApp::window_height,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);

	heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	//深度バッファの生成
	result = spritecomon->GetDxInitialize()->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		IID_PPV_ARGS(&depthBuff)
	);
	assert(SUCCEEDED(result));
}

void PostEffect::CreateDSV()
{
	//DSV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;

	//DSV用デスクリプタヒープを作成
	result = spritecomon->GetDxInitialize()->GetDevice()->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	//デスクリプタヒープにDSV作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	spritecomon->GetDxInitialize()->GetDevice()->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart()
	);
}

void PostEffect::Draw()
{
	matRot = Affin::matUnit();
	matRot *= Affin::matRotateZ(XMConvertToRadians(rotation));//Z軸周りに0度回転してから
	matTrans = Affin::matTrans(position.x, position.y, 0.0f);//(-50,0,0)平行移動

	matWorld = Affin::matUnit();//変形をリセット
	matWorld *= matRot;//ワールド行列にスケーリングを反映
	matWorld *= matTrans;

	// 定数バッファにデータ転送
	HRESULT result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	if (SUCCEEDED(result))
	{
		constMapTransform->mat = matWorld * matProjection;	// 行列の合成	
	}

	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);
	if (SUCCEEDED(result))
	{
		constMapMaterial->color = color;
	}

	spritecomon->SetTextureCommandsPost(textureIndex_,descHeapSRV);

	//頂点バッファビューの設定コマンド
	spritecomon->GetDxInitialize()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	// 定数バッファビュー(CBV)の設定コマンド
	spritecomon->GetDxInitialize()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	// 定数バッファビュー(CBV)の設定コマンド
	spritecomon->GetDxInitialize()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	// 描画コマンド
	spritecomon->GetDxInitialize()->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0); // 全ての頂点を使って描画

}

void PostEffect::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	//リソースバリアを変更(シェーダーリソース->描画可能)
	resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	cmdList->ResourceBarrier(1, &resourceBarrier);

	//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
	rtvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();

	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();

	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//ビューポートの設定
	
	//viewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height);
	
	//CD3DX12_VIEWPORT viewPort2;
	//viewPort2.Width = winW;//横幅
	//viewPort2.Height = winH;//縦幅
	//viewPort2.TopLeftX = 0;//左上X
	//viewPort2.TopLeftY = 0;//左上Y
	//viewPort2.MinDepth = 0.0f;//最小深度（０でよい）
	//viewPort2.MaxDepth = 1.0f;//最大深度（１でよい）

	//cmdList->RSSetViewports(1, &);

	//シザリング矩形の設定
	cmdList->RSSetScissorRects(1, &rect);

	//全画面クリア
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

	//深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	//リソースバリアを変更(描画可能->シェーダーリソース)
	cmdList->ResourceBarrier(1, &resourceBarrier);

}
