#include "PlayerBullet.h"
#include <cassert>


PlayerBullet::PlayerBullet()
{

}

PlayerBullet::~PlayerBullet()
{

}

void PlayerBullet::Initalize(Model* model, const Vector3& position, 
	const Vector3& velocity)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("player_bullet.png");

	// ���[���h�g�����X�t�H�[���̏�����
	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	// �����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

	vectorChange_ = new VectorChange();

	// �����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;

}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerBullet::Update()
{
	// ���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
	worldTransform_.translation_ += velocity_;
	vectorChange_->MyUpdate(worldTransform_);
	// ���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

Vector3 PlayerBullet::GetRadius()
{
	Vector3 PlayerBulletRadius;
	// ���̑傫���𔼕��ɂ��Ĕ��a�ɂ���
	PlayerBulletRadius.x = worldTransform_.scale_.x / 2;
	// �c�̑傫���𔼕��ɂ��Ĕ��a�ɂ���
	PlayerBulletRadius.y = worldTransform_.scale_.y / 2;
	// ���s���̑傫���𔼕��ɂ��Ĕ��a�ɂ���
	PlayerBulletRadius.z = worldTransform_.scale_.z / 2;

	return PlayerBulletRadius;
}
