#pragma once
/// <summary>
/// ���삵�����w�N���X
/// </summary>
class MyMath {
public:
	// �R���X�g���N�^
	MyMath();
	// �f�X�g���N�^
	~MyMath();

	// �����o�֐�

	/// <summary>
	/// �ŏ��l
	/// </summary>
	/// <param name="num"></param>
	/// <param name="num2"></param>
	/// <returns></returns>
	float MinNum(float min, float num);
	float MaxNum(float max, float num);
	float Clamp(float min, float max, float num);
	float ConvertToRadian(float degree);
};
