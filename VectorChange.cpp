#include "VectorChange.h"
#include <DirectXMath.h>

VectorChange::VectorChange()
{

}

VectorChange::~VectorChange()
{
	
}

Matrix4 VectorChange :: MyScale(WorldTransform& worldTransform_)
{
	//// X,Y,Z�����̃X�P�[�����O��ݒ�(�ǂꂭ�炢�����L�΂��́H)
	//worldTransform_.scale_ = { 5,5,5 };
	// �X�P�[�����O�s���錾(�Z��)
	Matrix4 matScale;

	// �P�ʍs��ɕϊ�
	matScale.IdentityMatrix();

	// �X�P�[�����O�{�����s��ɐݒ肷��
	matScale.m[0][0] = worldTransform_.scale_.x;
	matScale.m[1][1] = worldTransform_.scale_.y;
	matScale.m[2][2] = worldTransform_.scale_.z;
	matScale.m[3][3] = 1;

	return matScale;
}

Matrix4 VectorChange:: MyRotationX(WorldTransform& worldTransform_)
{
	// �e���p��]�s���錾
	Matrix4 matRotX;

	// �P�ʍs��ɕϊ�
	matRotX.IdentityMatrix();
	// X����]�s��̊e�v�f��ݒ肷��
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cosf(worldTransform_.rotation_.x);
	matRotX.m[1][2] = sinf(worldTransform_.rotation_.x);
	matRotX.m[2][1] = -sinf(worldTransform_.rotation_.x);
	matRotX.m[2][2] = cosf(worldTransform_.rotation_.x);
	matRotX.m[3][3] = 1;

	return matRotX;
}

Matrix4 VectorChange::MyRotationY(WorldTransform& worldTransform_)
{
	// �e���p��]�s���錾
	Matrix4 matRotY;

	matRotY.IdentityMatrix();
	// Y����]�s��̊e�v�f��ݒ肷��
	matRotY.m[0][0] = cosf(worldTransform_.rotation_.y);
	matRotY.m[0][2] = -sinf(worldTransform_.rotation_.y);
	matRotY.m[1][1] = 1;
	matRotY.m[2][0] = sinf(worldTransform_.rotation_.y);
	matRotY.m[2][2] = cosf(worldTransform_.rotation_.y);
	matRotY.m[3][3] = 1;

	return matRotY;
}

Matrix4 VectorChange::MyRotationZ(WorldTransform& worldTransform_)
{
	// �e���p��]�s���錾
	Matrix4 matRotZ;

	matRotZ.IdentityMatrix();
	// Z����]�s��̊e�v�f��ݒ肷��
	matRotZ.m[0][0] = cosf(worldTransform_.rotation_.z);
	matRotZ.m[0][1] = sinf(worldTransform_.rotation_.z);
	matRotZ.m[1][0] = -sinf(worldTransform_.rotation_.z);
	matRotZ.m[1][1] = cosf(worldTransform_.rotation_.z);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	return matRotZ;
}

Matrix4 VectorChange::MyTranslation(WorldTransform& worldTransform_)
{

	//// ���s�ړ��s���錾
	//Matrix4 matTrans = MathUtility::Matrix4Identity();

	Matrix4 matTrans;
	matTrans.IdentityMatrix();

	// �ړ��ʂ��s��ɐݒ肷��
	matTrans.m[0][0] = 1;
	matTrans.m[1][1] = 1;
	matTrans.m[2][2] = 1;
	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;
	matTrans.m[3][3] = 1;

	return matTrans;

}

Matrix4 VectorChange::MyUpdate(WorldTransform& worldTransform_)
{
	Matrix4 mat;
	mat.IdentityMatrix();
	worldTransform_.matWorld_ = mat;

	// �X�P�[�����O
	Matrix4 matScale = VectorChange::MyScale(worldTransform_);
	Matrix4 matRotationZ = VectorChange::MyRotationZ(worldTransform_);
	Matrix4 matRotationX = VectorChange::MyRotationX(worldTransform_);;
	Matrix4 matRotationY = VectorChange::MyRotationY(worldTransform_);
	Matrix4 matTrans = VectorChange::MyTranslation(worldTransform_);

	worldTransform_.matWorld_ *= matScale;

	worldTransform_.matWorld_ *= matRotationZ;


	// ��]�pZ
	worldTransform_.matWorld_ *= matRotationZ;
	// ��]�pX
	worldTransform_.matWorld_ *= matRotationX;
	// ��]�pY
	worldTransform_.matWorld_ *= matRotationY;
	// ���s�ړ�
	worldTransform_.matWorld_ *= matTrans;

	// �s��̓]��
	worldTransform_.TransferMatrix();

	return worldTransform_.matWorld_;
}