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
	matRot *= Affin::matRotateZ(XMConvertToRadians(rotation));//Z�������0�x��]���Ă���
	matTrans = Affin::matTrans(position.x, position.y, 0.0f);//(-50,0,0)���s�ړ�

	matWorld = Affin::matUnit();//�ό`�����Z�b�g
	matWorld *= matRot;//���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matTrans;


	// �萔�o�b�t�@�Ƀf�[�^�]��
	HRESULT result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	if (SUCCEEDED(result))
	{
		constMapTransform->mat = matWorld * matProjection;	// �s��̍���	
	}

	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);
	if (SUCCEEDED(result))
	{
		constMapMaterial->color = color;
	}

	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelineState.Get())

	spritecomon->SetTextureCommands(textureIndex_);

	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	spritecomon->GetDxInitialize()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	spritecomon->GetDxInitialize()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	spritecomon->GetDxInitialize()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	// �`��R�}���h
	spritecomon->GetDxInitialize()->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
}
