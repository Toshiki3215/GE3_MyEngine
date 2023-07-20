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
    //�R���X�g���N�^
    PostEffect();

    //������
    void Initialize(SpriteCommon* spritecommon_);

    void CreateTexBuff();

    void CreateSRV();

    void CreateRTV();

    void CreateBuff();

    void CreateDSV();

    //�`��R�}���h�̔��s
    void Draw();

    //�V�[���`��O����
    void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

    //�V�[���`��㏈��
    void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

private:
    // ���[�g�V�O�l�`��
    ComPtr<ID3D12RootSignature> rootSignature;

    // �p�C�v�����X�e�[�g�̐���
    ComPtr<ID3D12PipelineState> pipelineState;

    //�e�N�X�`���o�b�t�@
    ComPtr<ID3D12Resource> texBuff;

    //SRV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;

    CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

    CD3DX12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor);

    CD3DX12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,D3D12_RESOURCE_STATE_RENDER_TARGET);

    CD3DX12_VIEWPORT viewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height);

    CD3DX12_RECT rect = CD3DX12_RECT(0, 0, WinApp::window_width, WinApp::window_height);

    ////�e�N�X�`���ԍ�
    //uint32_t textureIndex_ = 0;

    UINT incrementSize;

    //�[�x�o�b�t�@
    ComPtr<ID3D12Resource> depthBuff;

    //RTV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;

    //DSV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    //��ʃN���A�J���[
    static const float clearColor[4];

};

