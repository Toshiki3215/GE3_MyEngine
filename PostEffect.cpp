#include "PostEffect.h"
#include <d3dx12.h>

using namespace DirectX;

PostEffect::PostEffect()
	:Sprite()
{
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
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

	//パイプラインステートの設定
	cmdList->SetPipelineState(pipelineState.Get())

	spritecomon->SetTextureCommands(textureIndex_);

	//頂点バッファビューの設定コマンド
	spritecomon->GetDxInitialize()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	// 定数バッファビュー(CBV)の設定コマンド
	spritecomon->GetDxInitialize()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	// 定数バッファビュー(CBV)の設定コマンド
	spritecomon->GetDxInitialize()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	// 描画コマンド
	spritecomon->GetDxInitialize()->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0); // 全ての頂点を使って描画
}
