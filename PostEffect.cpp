#include "PostEffect.h"

PostEffect::PostEffect()
	: Sprite(
		100,			//�e�N�X�`���ԍ�
		{0,0},			//���W
		{500.0f,500.0f},//�T�C�Y
		{1,1,1,1},		//�F
		{0.0f,0.0f},	//�A���J�[�|�C���g
		false,			//���E���]�t���O
		false)			//�㉺���]�t���O
{
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
}
