// ConvertXM.h
// DirectXMath‚Æ©ì‚Ì”ŠwƒNƒ‰ƒX‚Ì•ÏŠ·

#pragma once
#include <DirectXMath.h>
#include "Matrix4.h"

using namespace DirectX;

class ConvertXM
{

public:

	/// <summary>
	/// XMMatrix‚©‚çMatrix4‚É•ÏŠ·
	/// </summary>
	/// <param name="XMMat"></param>
	/// <returns></returns>
	static Matrix4 ConvertXMMATtoMat4( XMMATRIX XMMat);

	/// <summary>
	/// Matrix4‚©‚çXMMatrix‚É•ÏŠ·
	/// </summary>
	/// <param name="XMMat"></param>
	/// <returns></returns>
	static XMMATRIX ConvertMat4toXMMAT(Matrix4 Mat4);

	/// <summary>
	/// XMFLOAT3‚ğVector3‚É•ÏŠ·
	/// </summary>
	/// <param name="XMFlo3"></param>
	/// <returns></returns>
	static Vector3 ConvertXMFlo3toVec3(XMFLOAT3 XMFlo3);

	/// <summary>
	/// Vector3‚ğXMFLOAT3‚É•ÏŠ·
	/// </summary>
	/// <param name="vec3"></param>
	/// <returns></returns>
	static XMFLOAT3 ConvertVec3toXMFlo3(Vector3 vec3);

private:
	
	XMMATRIX xmMat;
	Matrix4 mat4;

	XMFLOAT3 xmFlo3;
	Vector3 vec3;

};

