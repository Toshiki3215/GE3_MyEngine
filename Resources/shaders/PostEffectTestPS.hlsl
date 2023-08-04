#include "PostEffectTest.hlsli"

Texture2D<float4> tex0 : register(t0);	// 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1);	// 1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		// 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

// �G���g���[�|�C���g
float4 main(VSOutput input) : SV_TARGET
{
	float4 colortex0 = tex0.Sample(smp, input.uv);
	
	//�s�N�Z��
	float coordinateU = 1.f / 1280.f;
	float coordinateV = 1.f / 720.f;

	//�s�N�Z��������
	int Num = 10;

	//�F���]
	float4 color = { 1 - colortex0.rgb, 1 };

	//���ςڂ���
	if (fmod(input.uv.y, 0.1f) < 0.05f)
	{
		color = (0, 0, 0, 0);

		for (int i = 0; i < Num; i++)
		{
			for (int j = 0; j < Num; j++)
			{
				color += tex1.Sample(smp,input.uv - float2(coordinateU * 5, coordinateV * 5) + float2(coordinateU * i, coordinateV * j));
			}
		}
		color.rgb /= (Num * Num);
	}

	return float4(color.rgb, 1);

}