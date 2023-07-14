#pragma once
#include "Sprite.h"
#include "SpriteCommon.h"
#include "DirectXInitialize.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Affin.h"
#include <d3d12.h>
#include <d3dx12.h>

class PostEffect :
    public Sprite
{
public:
    //コンストラクタ
    PostEffect();

    //初期化
    void Initialize(SpriteCommon* spritecommon_);

    //描画コマンドの発行
    void Draw(ID3D12GraphicsCommandList* cmdList);

private:
    // ルートシグネチャ
    ComPtr<ID3D12RootSignature> rootSignature;

    // パイプランステートの生成
    ComPtr<ID3D12PipelineState> pipelineState;

    //テクスチャバッファ
    ComPtr<ID3D12Resource> texBuff;

    //SRV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;

    CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

    ////テクスチャ番号
    //uint32_t textureIndex_ = 0;

    UINT incrementSize;

};

