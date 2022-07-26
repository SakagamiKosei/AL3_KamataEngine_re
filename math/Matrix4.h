#pragma once
#include "Vector3.h"
/// <summary>
/// 行列
/// </summary>
class Matrix4 {
  public:
	// 行x列
	float m[4][4];

	// コンストラクタ
	Matrix4();
	// 成分を指定しての生成
	Matrix4(
	  float m00, float m01, float m02, float m03,
	  float m10, float m11, float m12, float m13,
	  float m20, float m21, float m22, float m23,
	  float m30, float m31, float m32, float m33);

	// 代入演算子オーバーロード
	Matrix4& operator*=(const Matrix4& m2);

	Matrix4 CreateIdentityMatrix()
	{
		Matrix4 Mat;

		Mat.m[0][0] = 1; Mat.m[0][1] = 0; Mat.m[0][2] = 0; Mat.m[0][3] = 0;
		Mat.m[1][0] = 0; Mat.m[1][1] = 1; Mat.m[1][2] = 0; Mat.m[1][3] = 0;
		Mat.m[2][0] = 0; Mat.m[2][1] = 0; Mat.m[2][2] = 1; Mat.m[2][3] = 0;
		Mat.m[3][0] = 0; Mat.m[3][1] = 0; Mat.m[3][2] = 0; Mat.m[3][3] = 1;

		return Mat;

	}



	void IdentityMatrix()
	{
		m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
		m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
		m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
		m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
	}

	Vector3 transform(const Vector3& v, const Matrix4& m)
	{
		float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];
		Vector3 result
		{
			(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
			(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
			(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w
		};
		return result;
	}

};
