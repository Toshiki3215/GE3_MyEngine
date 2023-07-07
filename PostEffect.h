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
    //�R���X�g���N�^
    PostEffect();

    //�`��R�}���h�̔��s
    //void Draw(ID3D12GraphicsCommandList* cmdList);

private:
    // ���[�g�V�O�l�`��
    ComPtr<ID3D12RootSignature> rootSignature;

    // �p�C�v�����X�e�[�g�̐���
    ComPtr<ID3D12PipelineState> pipelineState;

};

