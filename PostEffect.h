#pragma once
#include "Sprite.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Affin.h"

class PostEffect :
    public Sprite
{
public:
    //コンストラクタ
    PostEffect();

    //描画コマンドの発行
    //void Draw(ID3D12GraphicsCommandList* cmdList);

private:
    // ルートシグネチャ
    ComPtr<ID3D12RootSignature> rootSignature;

    // パイプランステートの生成
    ComPtr<ID3D12PipelineState> pipelineState;

};

