#pragma once
/// <summary>
/// 当たり判定クラス
/// </summary>
class Collider
{
public:
	Collider();
	~Collider();

	/// <summary>
	/// 球と球の当たり判定を取り判定結果を返す関数
	/// </summary>
	/// <returns></returns>
	bool OnBallCollision(int posA_x,int posA_y, int posA_z,int radiusA,
		int posB_x, int posB_y, int posB_z, int radiusB);
};