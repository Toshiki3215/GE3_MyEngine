#include "PostEffect.h"
#include <d3dx12.h>
#include "WindowsApp.h"

using namespace DirectX;

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
		nullptr,
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

	//spritecomon->InitializePost();

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

	{
		//ID3D12DescriptorHeap* ppHeap[] = { descHeapSRV.Get() };

		////パイプラインステートの設定
		//cmdList->SetPipelineState(pipelineState.Get());

		////ルートシグネチャの設定
		//cmdList->SetComputeRootSignature(rootSignature.Get());

		////プリミティブ形状を設定
		//cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		//// SRVヒープの設定コマンド
		//cmdList->SetDescriptorHeaps(1, descHeapSRV.GetAddressOf());
		//// SRVヒープの先頭ハンドルを取得（SRVを指しているはず）
		//D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descHeapSRV->GetGPUDescriptorHandleForHeapStart();
		//// SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
		//srvGpuHandle.ptr += (incrementSize * textureIndex_);

		//cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

		////頂点バッファビューの設定コマンド
		//cmdList->IASetVertexBuffers(0, 1, &vbView);
		//// 定数バッファビュー(CBV)の設定コマンド
		//cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
		//// 定数バッファビュー(CBV)の設定コマンド
		//cmdList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
		//// 描画コマンド
		//cmdList->DrawInstanced(_countof(vertices), 1, 0, 0); // 全ての頂点を使って描画
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
