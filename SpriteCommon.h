#pragma once
#include<DirectXMath.h>

class SpriteCommon
{
private: 

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;


public:

	//頂点データ構造体
	struct Vertex
	{
		XMFLOAT3 pos;    //x,y,z座標
		XMFLOAT3 normal; //法線ベクトル
		XMFLOAT2 uv;     //u,v座標
	};

	//定数バッファ用データ構造体(マテリアル)
	struct ConstBufferDataMaterial
	{
		//色(RGBA)
		XMFLOAT4 color;
	};

	//定数バッファ用データ構造体(3D変換行列)
	struct ConstBufferDataTransform
	{
		//3D変換行列
		XMMATRIX mat;
	};

	void Initialize();

};

