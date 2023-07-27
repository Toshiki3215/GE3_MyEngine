#include "PostEffect.h"
#include <d3dx12.h>
#include "WindowsApp.h"

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

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
	spritecomon = spritecommon_;

	//基底クラスとしての初期化
	Sprite::Initialize(spritecomon);

	//パイプライン生成
	//CreateGraphicsPipelineState();

	//頂点バッファ生成
	//result = spritecomon->GetDxInitialize()->GetDevice()->CreateCommittedResource(
	//	&heapProp, // ヒープ設定
	//	D3D12_HEAP_FLAG_NONE,
	//	&spritecomon->GetResourceDesc(), // リソース設定
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&vertBuff));
	//assert(SUCCEEDED(result));

	////頂点データ
	//VertexPosUv 

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
	rtvH = descHeapRTV->GetCPUDescriptorHandleForHeapStart();

	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();

	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//ビューポートの設定
	
	viewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height);
	
	//CD3DX12_VIEWPORT viewPort2;
	//viewPort2.Width = winW;//横幅
	//viewPort2.Height = winH;//縦幅
	//viewPort2.TopLeftX = 0;//左上X
	//viewPort2.TopLeftY = 0;//左上Y
	//viewPort2.MinDepth = 0.0f;//最小深度（０でよい）
	//viewPort2.MaxDepth = 1.0f;//最大深度（１でよい）

	cmdList->RSSetViewports(1, &viewPort);

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

void PostEffect::CreateGraphicsPipelineState()
{
	HRESULT result = S_FALSE;

	//頂点シェーダオブジェクト
	ComPtr<ID3DBlob> vsBlob;
	//ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;
	//エラーオブジェクト
	ComPtr<ID3DBlob> errorBlob;

	//頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/PostEffectTestVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	// エラーなら
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/PostEffectTestPS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	// エラーなら
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ // xyz座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}, // (1行で書いたほうが見やすい)
	};

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
	// シェーダーの設定
	gPipeline.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	gPipeline.VS.BytecodeLength = vsBlob->GetBufferSize();
	gPipeline.PS.pShaderBytecode = psBlob->GetBufferPointer();
	gPipeline.PS.BytecodeLength = psBlob->GetBufferSize();

	// サンプルマスクの設定
	gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	gPipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // カリングしない
	//gPipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	//gPipeline.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	// デプスステンシルステート
	gPipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	//ブレンドステートの設定
	gPipeline.BlendState.RenderTarget[0] = blenddesc;

	//深度バッファのフォーマット
	gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//頂点レイアウトの設定
	gPipeline.InputLayout.pInputElementDescs = inputLayout;
	gPipeline.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状設定
	gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gPipeline.NumRenderTargets = 1;
	gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gPipeline.SampleDesc.Count = 1;

	// デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;         //一度の描画に使うテクスチャが1枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;     //テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[2] = {};

	// 定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   // 種類
	rootParams[0].Descriptor.ShaderRegister = 0;                   // 定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;                    // デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;  // 全てのシェーダから見える

	// テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   //種類
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;		  //デスクリプタレンジ
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;              		  //デスクリプタレンジ数
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //横繰り返し（タイリング）
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //縦繰り返し（タイリング）
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //奥行繰り返し（タイリング）
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;  //ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;                   //全てリニア補間
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;                                 //ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;                                              //ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;           //ピクセルシェーダからのみ使用可能

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	//rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	//rootSignatureDesc.pParameters = rootParams;				//ルートパラメータの先頭アドレス
	//rootSignatureDesc.NumParameters = _countof(rootParams);	//ルートパラメータ数
	//rootSignatureDesc.pStaticSamplers = &samplerDesc;
	//rootSignatureDesc.NumStaticSamplers = 1;

}
