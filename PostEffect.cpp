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

	//���N���X�Ƃ��Ă̏�����
	Sprite::Initialize(spritecomon);

	//�e�N�X�`�����\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WinApp::window_width,
		(UINT)WinApp::window_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//�e�N�X�`���o�b�t�@�̐���
	result = spritecomon->GetDxInitialize()->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);

	assert(SUCCEEDED(result));

	{//�e�N�X�`����ԃN���A
		//��f��(1280 �~ 720 = 921600�s�N�Z��)
		const UINT pixelCount = WinApp::window_width * WinApp::window_height;

		//�摜1�s���̃f�[�^�T�C�Y
		const UINT rowPitch = sizeof(UINT) * WinApp::window_width;

		//�摜�S�̂̃f�[�^�T�C�Y
		const UINT depthPitch = rowPitch * WinApp::window_height;

		//�摜�C���[�W
		UINT* img = new UINT[pixelCount];
		for (int i = 0; i < pixelCount; i++)
		{
			img[i] = 0xff0000ff;	//R�Fff,G�F00,B�F00,A�Fff
		}

		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);

		assert(SUCCEEDED(result));
		delete[] img;

	}

	//spritecomon->InitializePost();

	{
		//SRV�p�f�X�N���v�^�q�[�v�ݒ�
		D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
		srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		srvDescHeapDesc.NumDescriptors = 1;

		//SRV�p�f�X�N���v�^�q�[�v�𐶐�
		result = spritecomon->GetDxInitialize()->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
		assert(SUCCEEDED(result));

		//SRV�ݒ�
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//�ݒ�\����
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		//�f�X�N���v�^�q�[�v��SRV�쐬
		spritecomon->GetDxInitialize()->GetDevice()->CreateShaderResourceView(
			texBuff.Get(),	//�r���[�Ɗ֘A�t����o�b�t�@
			&srvDesc,
			descHeapSRV->GetCPUDescriptorHandleForHeapStart()
		);
	}


}

void PostEffect::Draw()
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

	{
		//ID3D12DescriptorHeap* ppHeap[] = { descHeapSRV.Get() };

		////�p�C�v���C���X�e�[�g�̐ݒ�
		//cmdList->SetPipelineState(pipelineState.Get());

		////���[�g�V�O�l�`���̐ݒ�
		//cmdList->SetComputeRootSignature(rootSignature.Get());

		////�v���~�e�B�u�`���ݒ�
		//cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		//// SRV�q�[�v�̐ݒ�R�}���h
		//cmdList->SetDescriptorHeaps(1, descHeapSRV.GetAddressOf());
		//// SRV�q�[�v�̐擪�n���h�����擾�iSRV���w���Ă���͂��j
		//D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descHeapSRV->GetGPUDescriptorHandleForHeapStart();
		//// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
		//srvGpuHandle.ptr += (incrementSize * textureIndex_);

		//cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

		////���_�o�b�t�@�r���[�̐ݒ�R�}���h
		//cmdList->IASetVertexBuffers(0, 1, &vbView);
		//// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		//cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
		//// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		//cmdList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
		//// �`��R�}���h
		//cmdList->DrawInstanced(_countof(vertices), 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
	}

	spritecomon->SetTextureCommandsPost(textureIndex_,descHeapSRV);

	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	spritecomon->GetDxInitialize()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	spritecomon->GetDxInitialize()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	spritecomon->GetDxInitialize()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	// �`��R�}���h
	spritecomon->GetDxInitialize()->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0); // �S�Ă̒��_���g���ĕ`��

}
