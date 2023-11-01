//Easing.cpp
//イージング関数用クラス

#include "Easing.h"

float Easing::easeIn(float time, float startPos, float endPos, float maxTime)
{
	float x = time / maxTime;
	float v = pow3(x);
	float z = endPos - startPos;
	float ret = z * v + startPos;

	return ret;
}

float Easing::easeOut(float time, float startPos, float endPos, float maxTime)
{
	float x = time / maxTime;
	float v = 1 - pow3(1 - x);
	float z = endPos - startPos;
	float ret = z * v + startPos;

	return ret;
}

float Easing::easeInBack(float time, float startPos, float endPos, float maxTime)
{
	float x = time / maxTime;

	float c1 = 1.70158f;
	float c2 = c1 + 1;
	float v = c2 * x * x * x - c1 * x * x;

	float z = endPos - startPos;
	float ret = z * v + startPos;

	return ret;
}

float Easing::pow3(float x)
{
	return x * x * x;
}

float Easing::pow4(float x)
{
	return x * x * x * x;
}

float Easing::pow5(float x)
{
	return x * x * x * x * x;
}