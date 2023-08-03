#pragma once
#include "DirectXInitialize.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Matrix4.h"
class PostEffect
{
public:

    /// <summary>
    /// ���_�f�[�^�\����
    /// </summary>
    struct VertexPosUv {
        Vector3 pos; // xyz���W
        Vector2 uv;  // uv���W
    };

    /// <summary>
    /// �萔�o�b�t�@�p�f�[�^�\����
    /// </summary>
    struct ConstBufferData {
        Vector4 color; // �F (RGBA)
        Matrix4 mat;   // �R�c�ϊ��s��
    };

    //�|�X�g�G�t�F�N�g�Ɋւ���GPU�ɑ��肽�����̂܂Ƃ�
    struct SendDataToGPU {
        int shadeNumber;
        int kernelSize;
        Vector2 center;
        float intensity;
        int samples;
        float _AngleDeg;
        float _AngleDeg2;
    };

    static void Initialize(DirectXInitialize* dxInit, const uint32_t& WinWidth, const uint32_t& WinHeight);

    static void Finalize();

    /// <summary>
    /// �p�C�v���C������
    /// </summary>
    static void CreatGraphicsPipelineState();

    /// <summary>
    /// �V�[���`��O����
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    static void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

    static void Draw(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// �V�[���`��㏈��
    /// </summary>
    /// <param name="cmdList">�R�}���h����</param>
    static void PostDrawScene();

    static void SetShadeNumber(int SetShadeNumber);

    static void SetKernelSize(int range);

    static void SetRadialBlur(Vector2 senter, float intensity, int sample);

    static void SetAngle(float angle1, float angle2);



private://�ÓI�����o�ϐ�
    static const float clearColor[4];

    static ID3D12Device* device_;

    static ID3D12GraphicsCommandList* commandList;

    static VertexPosUv vertices[4];

    static VertexPosUv* vertMap;

    static Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;	//���_�o�b�t�@

    //���_�o�b�t�@�r���[�̍쐬
    static D3D12_VERTEX_BUFFER_VIEW vbView;
    static Microsoft::WRL::ComPtr<ID3D12Resource> texBuff[2];

    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    //�[�x�o�b�t�@
    static Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
    //RTV�p�̃f�X�N���v�^�q�[�v
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV�p�̃f�X�N���v�^�q�[�v
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
    static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
private:
    // �萔�o�b�t�@
    static Microsoft::WRL::ComPtr<ID3D12Resource> constDataBuff_;
    // �}�b�s���O�ς݃A�h���X
    static SendDataToGPU* dataMap;
};