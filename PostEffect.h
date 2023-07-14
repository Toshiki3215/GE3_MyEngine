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

    //�`��R�}���h�̔��s
    void Draw(ID3D12GraphicsCommandList* cmdList);

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

    ////�e�N�X�`���ԍ�
    //uint32_t textureIndex_ = 0;

    UINT incrementSize;

};

