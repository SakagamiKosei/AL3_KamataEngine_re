#pragma once
/// <summary>
/// �����蔻��N���X
/// </summary>
class Collider
{
public:
	Collider();
	~Collider();

	/// <summary>
	/// ���Ƌ��̓����蔻�����蔻�茋�ʂ�Ԃ��֐�
	/// </summary>
	/// <returns></returns>
	bool OnBallCollision(int posA_x,int posA_y, int posA_z,int radiusA,
		int posB_x, int posB_y, int posB_z, int radiusB);
};