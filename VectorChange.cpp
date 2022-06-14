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
	//// X,Y,Z方向のスケーリングを設定(どれくらい引き伸ばすの？)
	//worldTransform_.scale_ = { 5,5,5 };
	// スケーリング行列を宣言(住所)
	Matrix4 matScale;

	// 単位行列に変換
	matScale.IdentityMatrix();

	// スケーリング倍率を行列に設定する
	matScale.m[0][0] = worldTransform_.scale_.x;
	matScale.m[1][1] = worldTransform_.scale_.y;
	matScale.m[2][2] = worldTransform_.scale_.z;
	matScale.m[3][3] = 1;

	return matScale;
}

Matrix4 VectorChange:: MyRotationX(WorldTransform& worldTransform_)
{
	// 各軸用回転行列を宣言
	Matrix4 matRotX;

	// 単位行列に変換
	matRotX.IdentityMatrix();
	// X軸回転行列の各要素を設定する
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
	// 各軸用回転行列を宣言
	Matrix4 matRotY;

	matRotY.IdentityMatrix();
	// Y軸回転行列の各要素を設定する
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
	// 各軸用回転行列を宣言
	Matrix4 matRotZ;

	matRotZ.IdentityMatrix();
	// Z軸回転行列の各要素を設定する
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

	//// 平行移動行列を宣言
	//Matrix4 matTrans = MathUtility::Matrix4Identity();

	Matrix4 matTrans;
	matTrans.IdentityMatrix();

	// 移動量を行列に設定する
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

	// スケーリング
	Matrix4 matScale = VectorChange::MyScale(worldTransform_);
	Matrix4 matRotationZ = VectorChange::MyRotationZ(worldTransform_);
	Matrix4 matRotationX = VectorChange::MyRotationX(worldTransform_);;
	Matrix4 matRotationY = VectorChange::MyRotationY(worldTransform_);
	Matrix4 matTrans = VectorChange::MyTranslation(worldTransform_);

	worldTransform_.matWorld_ *= matScale;

	worldTransform_.matWorld_ *= matRotationZ;


	// 回転角Z
	worldTransform_.matWorld_ *= matRotationZ;
	// 回転角X
	worldTransform_.matWorld_ *= matRotationX;
	// 回転角Y
	worldTransform_.matWorld_ *= matRotationY;
	// 平行移動
	worldTransform_.matWorld_ *= matTrans;

	// 行列の転送
	worldTransform_.TransferMatrix();

	return worldTransform_.matWorld_;
}