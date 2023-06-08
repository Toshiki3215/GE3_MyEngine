#include "PostEffect.h"

PostEffect::PostEffect()
	: Sprite(
		100,			//テクスチャ番号
		{0,0},			//座標
		{500.0f,500.0f},//サイズ
		{1,1,1,1},		//色
		{0.0f,0.0f},	//アンカーポイント
		false,			//左右反転フラグ
		false)			//上下反転フラグ
{
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
}
