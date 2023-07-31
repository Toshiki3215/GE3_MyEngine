#include "PostEffect.h"
#include <d3dx12.h>
#include "WindowsApp.h"

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

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
	spritecomon = spritecommon_;

	//���N���X�Ƃ��Ă̏�����
	Sprite::Initialize(spritecomon);

	//�p�C�v���C������
	CreateGraphicsPipelineState();

	////���_�o�b�t�@����
	//result = spritecomon->GetDxInitialize()->GetDevice()->CreateCommittedResource(
	//	&heapProp, // �q�[�v�ݒ�
	//	D3D12_HEAP_FLAG_NONE,
	//	&spritecomon->GetResourceDesc(), // ���\�[�X�ݒ�
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&vertBuff));
	//assert(SUCCEEDED(result));

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

		{
			////�e�N�X�`����ԃN���A
			////��f��(1280 �~ 720 = 921600�s�N�Z��)
			//const UINT pixelCount = WinApp::window_width * WinApp::window_height;

			////�摜1�s���̃f�[�^�T�C�Y
			//const UINT rowPitch = sizeof(UINT) * WinApp::window_width;

			////�摜�S�̂̃f�[�^�T�C�Y
			//const UINT depthPitch = rowPitch * WinApp::window_height;

			////�摜�C���[�W
			//UINT* img = new UINT[pixelCount];
			//for (int i = 0; i < pixelCount; i++)
			//{
			//	img[i] = 0xff0000ff;	//R�Fff,G�F00,B�F00,A�Fff
			//}

			////�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
			//result = texBuff->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);

			//assert(SUCCEEDED(result));
			//delete[] img;
		}
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

		//heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		//clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

		////�[�x�o�b�t�@�̐���
		//result = spritecomon->GetDxInitialize()->GetDevice()->CreateCommittedResource(
		//	&heapProp,
		//	D3D12_HEAP_FLAG_NONE,
		//	&depthResDesc,
		//	D3D12_RESOURCE_STATE_DEPTH_WRITE,
		//	&clearValue,
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

		spritecomon->SetTextureCommandsPost(textureIndex_,descHeapSRV, pipelineState,rootSignature);

		//���_�o�b�t�@�r���[�̐ݒ�R�}���h
		spritecomon->GetDxInitialize()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
		// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		spritecomon->GetDxInitialize()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
		// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		spritecomon->GetDxInitialize()->GetCommandList()->SetGraphicsRootConstantBufferView(1, constBuffTransform->GetGPUVirtualAddress());
		// �`��R�}���h
		spritecomon->GetDxInitialize()->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
	}

	{
		//commandList = cmdList;

		////�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		//commandList->SetPipelineState(pipelineState.Get());
		//commandList->SetGraphicsRootSignature(rootSignature.Get());

		////SRV�q�[�v�̐ݒ�R�}���h
		//ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
		//commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		////�v���~�e�B�u�`��̐ݒ�R�}���h
		//commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		////���_�o�b�t�@�r���[�̐ݒ�R�}���h
		//commandList->IASetVertexBuffers(0, 1, &vbView);

		////�摜�`��
		////SRV�q�[�v�̐擪�n���h�����擾�iSRV���w���Ă���͂��j
		//commandList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 0,
		//	device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

		//commandList->SetGraphicsRootDescriptorTable(2, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 1,
		//	device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

		//commandList->SetGraphicsRootConstantBufferView(0, constDataBuff_->GetGPUVirtualAddress());

		////�`��R�}���h
		//commandList->DrawInstanced(_countof(vertices), 1, 0, 0);//���ׂĂ̒��_���g���ĕ`��
	}
}

void PostEffect::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	//���\�[�X�o���A��ύX(�V�F�[�_�[���\�[�X->�`��\)
	resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	cmdList->ResourceBarrier(1, &resourceBarrier);

	//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	rtvH = descHeapRTV->GetCPUDescriptorHandleForHeapStart();

	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();

	//�����_�[�^�[�Q�b�g���Z�b�g
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//�r���[�|�[�g�̐ݒ�
	
	viewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height);
	
	//CD3DX12_VIEWPORT viewPort2;
	//viewPort2.Width = winW;//����
	//viewPort2.Height = winH;//�c��
	//viewPort2.TopLeftX = 0;//����X
	//viewPort2.TopLeftY = 0;//����Y
	//viewPort2.MinDepth = 0.0f;//�ŏ��[�x�i�O�ł悢�j
	//viewPort2.MaxDepth = 1.0f;//�ő�[�x�i�P�ł悢�j

	cmdList->RSSetViewports(1, &viewPort);

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

void PostEffect::CreateGraphicsPipelineState()
{
	HRESULT result;

	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr;//���_�V�F�[�_�[�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr;//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g

	ID3DBlob* errorBlob = nullptr;//�G���[�I�u�W�F�N�g

	//���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/PostEffectTestVS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 
		0,
		&vsBlob, &errorBlob);

	//�G���[�Ȃ�
	if (FAILED(result)) {
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		//�G���[���e���o�̓E�C���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
	//�s�N�Z���V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/PostEffectTestPS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);
	//�G���[�Ȃ�
	if (FAILED(result)) {
		//errorBlob������e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		//�G���[���e���o�̓E�C���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
	//���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
		"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0,
		},//vyz���W(��s�ŏ������ق������₷��)
		{
		"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0,
		},//uv���W(��s�ŏ������ق������₷��)
	};

	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	//�V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	//�T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//�|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true;//�[�x�O���b�s���O��L����

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV0;
	descRangeSRV0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	//CD3DX12_DESCRIPTOR_RANGE descRangeSRV1;
	//descRangeSRV1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1); // t1 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[2] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV0, D3D12_SHADER_VISIBILITY_ALL);
	//rootparams[2].InitAsDescriptorTable(1, &descRangeSRV1, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc =
		CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR); // s0 ���W�X�^
	/*samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;*/
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	//samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(
		_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	// ���[�g�V�O�l�`���̐���
	result = spritecomon->GetDxInitialize()->GetDevice()->CreateRootSignature(
		0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	pipelineDesc.pRootSignature = rootSignature.Get();


	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = false;//�u�����h�����ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g��

	////���Z����
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//�f�X�g�̒l��100%�g��
	////���Z����
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;//�f�X�g����\�[�X�����Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//�f�X�g�̒l��100%�g��
	////�F���]
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-�f�X�g�J���[�̒l
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;//�g��Ȃ�
	
	//����������
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l

	////�u�����h�X�e�[�g
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL;//RBGA���ׂẴ`�����l����`��
	
	//���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	//�}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1;//�`��Ώۂ͈��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0�`255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; //1�s�N�Z���ɂ����T���v�����O

	//�p�C�v�����X�X�e�[�g�̐���
	result = spritecomon->GetDxInitialize()->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

}
