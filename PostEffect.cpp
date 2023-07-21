#include "PostEffect.h"
#include <d3dx12.h>
#include "WindowsApp.h"

using namespace DirectX;

//�ÓI�����o�ϐ��̎���					  Red  Green Blue Alpha
const float PostEffect::clearColor[4] = { 0.25f,0.5f,0.1f,0.0f };	// �΂��ۂ��F

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

	CreateTexBuff();

	{
		////�e�N�X�`�����\�[�X�ݒ�
		//CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		//	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		//	WinApp::window_width,
		//	(UINT)WinApp::window_height,
		//	1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
		//);

		////�e�N�X�`���o�b�t�@�̐���
		//result = spritecomon->GetDxInitialize()->GetDevice()->CreateCommittedResource(
		//	&heapProp,
		//	D3D12_HEAP_FLAG_NONE,
		//	&texresDesc,
		//	D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		//	nullptr,
		//	IID_PPV_ARGS(&texBuff)
		//);

		//assert(SUCCEEDED(result));

		//{//�e�N�X�`����ԃN���A
		//	//��f��(1280 �~ 720 = 921600�s�N�Z��)
		//	const UINT pixelCount = WinApp::window_width * WinApp::window_height;

		//	//�摜1�s���̃f�[�^�T�C�Y
		//	const UINT rowPitch = sizeof(UINT) * WinApp::window_width;

		//	//�摜�S�̂̃f�[�^�T�C�Y
		//	const UINT depthPitch = rowPitch * WinApp::window_height;

		//	//�摜�C���[�W
		//	UINT* img = new UINT[pixelCount];
		//	for (int i = 0; i < pixelCount; i++)
		//	{
		//		img[i] = 0xff0000ff;	//R�Fff,G�F00,B�F00,A�Fff
		//	}

		//	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		//	result = texBuff->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);

		//	assert(SUCCEEDED(result));
		//	delete[] img;
		//}
	}

	// ----- SRV ----- //
	CreateSRV();

	{
		////SRV�p�f�X�N���v�^�q�[�v�ݒ�
		//D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
		//srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		//srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		//srvDescHeapDesc.NumDescriptors = 1;

		////SRV�p�f�X�N���v�^�q�[�v�𐶐�
		//result = spritecomon->GetDxInitialize()->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
		//assert(SUCCEEDED(result));

		////SRV�ݒ�
		//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//�ݒ�\����
		//srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		//srvDesc.Texture2D.MipLevels = 1;

		////�f�X�N���v�^�q�[�v��SRV�쐬
		//spritecomon->GetDxInitialize()->GetDevice()->CreateShaderResourceView(
		//	texBuff.Get(),	//�r���[�Ɗ֘A�t����o�b�t�@
		//	&srvDesc,
		//	descHeapSRV->GetCPUDescriptorHandleForHeapStart()
		//);
	}

	// ----- RTV ----- //
	CreateRTV();

	{
		////RTV�p�f�X�N���v�^�q�[�v�ݒ�
		//D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
		//rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		//rtvDescHeapDesc.NumDescriptors = 1;

		////RTV�p�f�X�N���v�^�q�[�v�𐶐�
		//result = spritecomon->GetDxInitialize()->GetDevice()->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
		//assert(SUCCEEDED(result));

		////�����_�[�^�[�Q�b�g�r���[�̐ݒ�
		//D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};

		////�V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
		//renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		//renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		////�f�X�N���v�^�q�[�v��RTV�쐬
		//spritecomon->GetDxInitialize()->GetDevice()->CreateRenderTargetView(
		//	texBuff.Get(),
		//	&renderTargetViewDesc,
		//	descHeapRTV->GetCPUDescriptorHandleForHeapStart()
		//);
	}

	// ----- �[�x�o�b�t�@ ----- //
	CreateBuff();

	{
		////�[�x�o�b�t�@���\�[�X�ݒ�
		//CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		//	DXGI_FORMAT_D32_FLOAT,
		//	WinApp::window_width,
		//	WinApp::window_height,
		//	1, 0,
		//	1, 0,
		//	D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		//);

		////�[�x�o�b�t�@�̐���
		//result = spritecomon->GetDxInitialize()->GetDevice()->CreateCommittedResource(
		//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		//	D3D12_HEAP_FLAG_NONE,
		//	&depthResDesc,
		//	D3D12_RESOURCE_STATE_DEPTH_WRITE,
		//	&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		//	IID_PPV_ARGS(&depthBuff)
		//);
		//assert(SUCCEEDED(result));
	}

	// ----- DSV ----- //
	CreateDSV();

	{
		////DSV�p�f�X�N���v�^�q�[�v�ݒ�
		//D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
		//DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		//DescHeapDesc.NumDescriptors = 1;

		////DSV�p�f�X�N���v�^�q�[�v���쐬
		//result = spritecomon->GetDxInitialize()->GetDevice()->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
		//assert(SUCCEEDED(result));

		////�f�X�N���v�^�q�[�v��DSV�쐬
		//D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		//dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		//dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		//spritecomon->GetDxInitialize()->GetDevice()->CreateDepthStencilView(
		//	depthBuff.Get(),
		//	&dsvDesc,
		//	descHeapDSV->GetCPUDescriptorHandleForHeapStart()
		//);
	}

}

void PostEffect::CreateTexBuff()
{
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
		&clearValue,
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
}

void PostEffect::CreateSRV()
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

void PostEffect::CreateRTV()
{
	//RTV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;

	//RTV�p�f�X�N���v�^�q�[�v�𐶐�
	result = spritecomon->GetDxInitialize()->GetDevice()->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));

	//�����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};

	//�V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	//�f�X�N���v�^�q�[�v��RTV�쐬
	spritecomon->GetDxInitialize()->GetDevice()->CreateRenderTargetView(
		texBuff.Get(),
		&renderTargetViewDesc,
		descHeapRTV->GetCPUDescriptorHandleForHeapStart()
	);
}

void PostEffect::CreateBuff()
{
	//�[�x�o�b�t�@���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		WinApp::window_width,
		WinApp::window_height,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);

	heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	//�[�x�o�b�t�@�̐���
	result = spritecomon->GetDxInitialize()->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		IID_PPV_ARGS(&depthBuff)
	);
	assert(SUCCEEDED(result));
}

void PostEffect::CreateDSV()
{
	//DSV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;

	//DSV�p�f�X�N���v�^�q�[�v���쐬
	result = spritecomon->GetDxInitialize()->GetDevice()->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	//�f�X�N���v�^�q�[�v��DSV�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	spritecomon->GetDxInitialize()->GetDevice()->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart()
	);
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

void PostEffect::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	//���\�[�X�o���A��ύX(�V�F�[�_�[���\�[�X->�`��\)
	resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	cmdList->ResourceBarrier(1, &resourceBarrier);

	//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	rtvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();

	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();

	//�����_�[�^�[�Q�b�g���Z�b�g
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//�r���[�|�[�g�̐ݒ�
	
	//viewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height);
	
	//CD3DX12_VIEWPORT viewPort2;
	//viewPort2.Width = winW;//����
	//viewPort2.Height = winH;//�c��
	//viewPort2.TopLeftX = 0;//����X
	//viewPort2.TopLeftY = 0;//����Y
	//viewPort2.MinDepth = 0.0f;//�ŏ��[�x�i�O�ł悢�j
	//viewPort2.MaxDepth = 1.0f;//�ő�[�x�i�P�ł悢�j

	//cmdList->RSSetViewports(1, &);

	//�V�U�����O��`�̐ݒ�
	cmdList->RSSetScissorRects(1, &rect);

	//�S��ʃN���A
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

	//�[�x�o�b�t�@�̃N���A
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	//���\�[�X�o���A��ύX(�`��\->�V�F�[�_�[���\�[�X)
	cmdList->ResourceBarrier(1, &resourceBarrier);

}
