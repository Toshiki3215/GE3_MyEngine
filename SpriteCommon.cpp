#include "SpriteCommon.h"
#include<DirectXMath.h>

void SpriteCommon::Initialize()
{

	//頂点データ
	Vertex vertices[] =
	{
		//  x     　y     z     法線   u    v
		//前
		{{  -5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},  //左下
		{{  -5.0f,  5.0f, -5.0f},{},{0.0f,0.0f}},  //左上
		{{   5.0f, -5.0f, -5.0f},{},{1.0f,1.0f}},  //右下
		{{   5.0f,  5.0f, -5.0f},{},{1.0f,0.0f}},  //右上

		//後					
		{{  -5.0f, -5.0f,  5.0f},{},{0.0f,1.0f}},  //左下
		{{  -5.0f,  5.0f,  5.0f},{},{0.0f,0.0f}},  //左上
		{{   5.0f, -5.0f,  5.0f},{},{1.0f,1.0f}},  //右下
		{{   5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},  //右上

		//左					
		{{  -5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},  //左下
		{{  -5.0f, -5.0f,  5.0f},{},{0.0f,0.0f}},  //左上
		{{  -5.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},  //右下
		{{  -5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},  //右上

		//右					
		{{   5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},  //左下
		{{   5.0f, -5.0f,  5.0f},{},{0.0f,0.0f}},  //左上
		{{   5.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},  //右下
		{{   5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},  //右上

		//下					
		{{  -5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},  //左下
		{{  -5.0f, -5.0f,  5.0f},{},{0.0f,1.0f}},  //左上
		{{   5.0f, -5.0f, -5.0f},{},{1.0f,1.0f}},  //右下
		{{   5.0f, -5.0f,  5.0f},{},{1.0f,1.0f}},  //右上

		//上					
		{{  -5.0f,  5.0f, -5.0f},{},{0.0f,1.0f}},  //左下
		{{  -5.0f,  5.0f,  5.0f},{},{0.0f,1.0f}},  //左上
		{{   5.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},  //右下
		{{   5.0f,  5.0f,  5.0f},{},{1.0f,1.0f}},  //右上
	};

	//インデックスデータ
	unsigned short indices[] =
	{
		//前
		0,1,2, //三角形1つ目
		2,1,3, //三角形2つ目

		//後
		5,4,6, //三角形3つ目
		5,6,7, //三角形4つ目

		//左
		8,9,10,
		10,9,11,

		//右
		13,12,14,
		13,14,15,

		//下
		17,16,18,
		17,18,19,

		//上
		20,21,22,
		22,21,23,
	};

	//法線の計算
	for (int i = 0; i < 36 / 3; i++)
	{
		//三角形1つごとに計算していく

		//三角形のインデックスを取り出して、一時的な変数に入れる
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];

		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);

		//p0→p1ベクトル、p0→p2ベクトルを計算 (ベクトルの減算)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		//外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);

		//正規化(長さを1にする)
		normal = XMVector3Normalize(normal);

		//求めた法線を頂点データに代入
		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);
	}

	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	ID3D12Resource* vertBuff = nullptr;
	DXInit.result = DXInit.device->CreateCommittedResource
	(&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(DXInit.result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	DXInit.result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(DXInit.result));

	// 全頂点に対して
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i]; // 座標をコピー
	}

	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();

	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;

	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	DXInit.result = D3DCompileFromFile
	(L"BasicVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	// エラーなら
	if (FAILED(DXInit.result))
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
	DXInit.result = D3DCompileFromFile
	(L"BasicPS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	// エラーなら
	if (FAILED(DXInit.result))
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
		//x,y,z座標
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},

		//法線ベクトル
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA},

		//u,v座標
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
	};

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; //インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//インデックスバッファの生成
	ID3D12Resource* indexBuff = nullptr;
	DXInit.result = DXInit.device->CreateCommittedResource
	(
		&heapProp, //ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, //リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	DXInit.result = indexBuff->Map(0, nullptr, (void**)&indexMap);

	//全てのインデックスに対して
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i]; //インデックスをコピー
	}

	//マッピング解除
	indexBuff->Unmap(0, nullptr);

	//インデックスバッファビューの生成
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;   // 背面をカリング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	//RGBA全てのチャンネルを描画(動作は上と同じ)
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;      //一度の描画に使うテクスチャが一枚なので１
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;  //テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// - ルートパラメータの設定 - //
	D3D12_ROOT_PARAMETER rootParams[3] = {};

	//  定数バッファ0番
	//定数バッファビュー
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;

	//定数バッファ番号
	rootParams[0].Descriptor.ShaderRegister = 0;

	//デフォルト値
	rootParams[0].Descriptor.RegisterSpace = 0;

	//全てのシェーダから見える
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// テクスチャレジスタ0番
	//種類
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;

	//デスクリプタレンジ
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;

	//デスクリプタレンジ数
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;

	//全てのシェーダから見える
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//  定数バッファ1番
	//種類
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;

	//定数バッファ番号
	rootParams[2].Descriptor.ShaderRegister = 1;

	//デフォルト値
	rootParams[2].Descriptor.RegisterSpace = 0;

	//全てのシェーダから見える
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;


	//テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};

	//横繰り返し(タイリング)
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;

	//縦繰り返し(タイリング)
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;

	//奥行繰り返し(タイリング)
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;

	//ボーダーの時は黒
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;

	//全てリニア補間
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;

	//ミニマップ最大値
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;

	//ミニマップ最小値
	samplerDesc.MinLOD = 0.0f;

	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;

	//ピクセルシェーダからのみ使用可能
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	// ルートシグネチャ
	ID3D12RootSignature* rootSignature;

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//ルートパラメータの先頭アドレス
	rootSignatureDesc.pParameters = rootParams;

	//ルートパラメータ数
	rootSignatureDesc.NumParameters = _countof(rootParams);

	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;
	DXInit.result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(DXInit.result));
	DXInit.result = DXInit.device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(DXInit.result));
	rootSigBlob->Release();

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature;

	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true;   //深度テストを行う
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;   //書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;   //小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;   //深度値フォーマット

	// パイプランステートの生成
	ID3D12PipelineState* pipelineState = nullptr;
	DXInit.result = DXInit.device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(DXInit.result));

	//0番の行列用定数バッファ
	ID3D12Resource* constBuffTransform0 = nullptr;
	ConstBufferDataTransform* constMapTransform0 = nullptr;

	//0番_定数バッファの生成(設定)
	{
		//ヒープ設定
		D3D12_HEAP_PROPERTIES cbHeapProp{};

		//GPUへの転送用
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

		//リソース設定
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

		//256バイトアラインメント
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		DXInit.result = DXInit.device->CreateCommittedResource
		(
			//ヒープ設定
			&cbHeapProp,
			D3D12_HEAP_FLAG_NONE,
			//リソース設定
			&cbResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform0)
		);

		DXInit.result = constBuffTransform0->Map(0, nullptr, (void**)&constMapTransform0);
		assert(SUCCEEDED(DXInit.result));
	}

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};

	//GPUへの転送用
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//256バイトアラインメント
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ID3D12Resource* constBuffMaterial = nullptr;

	//定数バッファの生成
	DXInit.result = DXInit.device->CreateCommittedResource
	(
		//ヒープ設定
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		//リソース設定
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial)
	);
	assert(SUCCEEDED(DXInit.result));

	//定数バッファのマッピング
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	//マッピング
	DXInit.result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);
	assert(SUCCEEDED(DXInit.result));

	//並行投影行列の計算
	constMapTransform0->mat = XMMatrixOrthographicOffCenterLH
	(
		0.0f, winApp.window_width,
		winApp.window_height, 0.0f,
		0.0f, 1.0f
	);

	//射影変換行列(透視投影)
	XMMATRIX matProjection = XMMatrixPerspectiveFovLH
	(
		//上下が画角45度
		XMConvertToRadians(45.0f),

		//アスペクト比(画面横幅 / 画面縦幅)
		(float)winApp.window_width / winApp.window_height,

		//前幅,奥幅
		0.1f, 1000.0f
	);

	//定数バッファにデータを転送する

	//値を書き込むと自動的に転送される
	constMapMaterial->color = XMFLOAT4(0.1f, 0.25f, 0.5f, 0.5f);

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	//WICテクスチャのロード
	DXInit.result = LoadFromWICFile
	(
		L"Resources/texture.png",  //「Resources」フォルダの「texture.png」
		WIC_FLAGS_NONE,
		&metadata, scratchImg
	);

	ScratchImage mipChain{};

	//ミニマップ生成
	DXInit.result = GenerateMipMaps
	(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain
	);

	if (SUCCEEDED(DXInit.result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	// --- テクスチャバッファ --- //
	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;   //幅
	textureResourceDesc.Height = (UINT)metadata.height;  //高さ
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	//テクスチャバッファの生成
	ID3D12Resource* texBuff = nullptr;
	DXInit.result = DXInit.device->CreateCommittedResource
	(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);

	//全ミニマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		//ミニマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);

		//テクスチャバッファにデータ転送
		DXInit.result = texBuff->WriteToSubresource
		(
			(UINT)i,
			//全領域へコピー
			nullptr,
			//元データアドレス
			img->pixels,
			//1ラインサイズ
			(UINT)img->rowPitch,
			//1枚サイズ
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(DXInit.result));
	}

	//SRVの最大個数
	const size_t kMaxSRVCount = 2056;

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;      //レンダーターゲットビュー
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;  //シェーダから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//設定を元のSRV用デスクリプタヒープを生成
	ID3D12DescriptorHeap* srvHeap = nullptr;
	DXInit.result = DXInit.device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(DXInit.result));

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//シェーダリソースビュー設定
	//設定構造体
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	//RGBA float
	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	//2Dテクスチャ
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	DXInit.device->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);


}