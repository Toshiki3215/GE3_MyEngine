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

    void CreateTexBuff();

    void CreateSRV();

    void CreateRTV();

    void CreateBuff();

    void CreateDSV();

    //描画コマンドの発行
    void Draw();

    //シーン描画前処理
    void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

    //シーン描画後処理
    void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

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

    CD3DX12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor);

    CD3DX12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,D3D12_RESOURCE_STATE_RENDER_TARGET);

    CD3DX12_VIEWPORT viewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height);

    CD3DX12_RECT rect = CD3DX12_RECT(0, 0, WinApp::window_width, WinApp::window_height);

    ////テクスチャ番号
    //uint32_t textureIndex_ = 0;

    UINT incrementSize;

    //深度バッファ
    ComPtr<ID3D12Resource> depthBuff;

    //RTV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;

    //DSV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    //画面クリアカラー
    static const float clearColor[4];

};

