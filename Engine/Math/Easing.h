#pragma once

class Easing
{
public:

	/// <summary>
	/// イーズイン関数
	/// </summary>
	/// <param name="time"></param>
	/// <param name="startPos"></param>
	/// <param name="endPos"></param>
	/// <param name="maxTime"></param>
	/// <returns></returns>
	float easeIn(float time, float startPos, float endPos, float maxTime);

	/// <summary>
	/// イーズアウト関数
	/// </summary>
	/// <param name="time"></param>
	/// <param name="startPos"></param>
	/// <param name="endPos"></param>
	/// <param name="maxTime"></param>
	/// <returns></returns>
	float easeOut(float time, float startPos, float endPos, float maxTime);

	float easeInBack(float time, float startPos, float endPos, float maxTime);

	/// <summary>
	/// 累乗(3乗):イージング用
	/// </summary>
	/// <param name="x"></param>
	/// <returns></returns>
	float pow2(float x);

	/// <summary>
	/// 累乗(3乗):イージング用
	/// </summary>
	/// <param name="x"></param>
	/// <returns></returns>
	float pow3(float x);

	/// <summary>
	/// 累乗(4乗):イージング用
	/// </summary>
	/// <param name="x"></param>
	/// <returns></returns>
	float pow4(float x);

	/// <summary>
	/// 累乗(5乗):イージング用
	/// </summary>
	/// <param name="x"></param>
	/// <returns></returns>
	float pow5(float x);
};