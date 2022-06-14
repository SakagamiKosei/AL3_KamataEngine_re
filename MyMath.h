#pragma once
/// <summary>
/// 自作した数学クラス
/// </summary>
class MyMath {
public:
	// コンストラクタ
	MyMath();
	// デストラクタ
	~MyMath();

	// メンバ関数

	/// <summary>
	/// 最小値
	/// </summary>
	/// <param name="num"></param>
	/// <param name="num2"></param>
	/// <returns></returns>
	float MinNum(float min, float num);
	float MaxNum(float max, float num);
	float Clamp(float min, float max, float num);
	float ConvertToRadian(float degree);
};
