#include "MyMath.h"

// コンストラクタ
MyMath::MyMath() {}
// デストラクタ
MyMath::~MyMath() {}

// メンバ関数
float MyMath::MinNum(float min, float num)
{
	// もし最小値より大きいなら
	if (num <= min)
	{
		// 最小値を返す
		return min;
	}
	// もし最小値より小さいなら
	// 最小値を返す
	return num;
}

float MyMath::MaxNum(float max, float num)
{
	// もし最大値より小さいなら
	if (max <= num)
	{
		// 最大値を返す
		return max;
	}
	// もし最大値より大きいなら
	// 最大値を返す
	return num;
}

float MyMath::Clamp(float min, float max, float num)
{
	// もし最小値より大きいなら
	if (num <= min)
	{
		// 最小値を返す
		return min;
	}
	// もし最大値より小さいなら
	else if (num >= max)
	{
		// 最大値を返す
		return max;
	}
	// これらの条件にあてはまらなければ
	// 値を返す
	return num;
}

float MyMath::ConvertToRadian(float degree)
{
	
	const float PI = 3.1415f;
	return degree * PI / 180;
}