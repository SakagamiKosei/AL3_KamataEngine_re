#include "MyMath.h"

// �R���X�g���N�^
MyMath::MyMath(){}
// �f�X�g���N�^
MyMath::~MyMath(){}

// �����o�֐�
float MyMath::MinNum(float min,float num)
{
	// �����ŏ��l���傫���Ȃ�
	if (min < num)
	{
		// �ŏ��l��Ԃ�
		return min;
	}
	// �����ŏ��l��菬�����Ȃ�
	// �ŏ��l��Ԃ�
	return num;
}

float MyMath::MaxNum(float max, float num)
{
	// �����ő�l��菬�����Ȃ�
	if (num < max)
	{
		// �ő�l��Ԃ�
		return max;
	}
	// �����ő�l���傫���Ȃ�
	// �ő�l��Ԃ�
	return num;
}

float Clamp(float min, float max, float num)
{
	// �����ŏ��l���傫���Ȃ�
	if (min < num)
	{
		// �ŏ��l��Ԃ�
		return min;
	}
	// �����ő�l��菬�����Ȃ�
	else if (num < max)
	{
		// �ő�l��Ԃ�
		return max;
	}
	// �����̏����ɂ��Ă͂܂�Ȃ����
	// �l��Ԃ�
	return num;
}