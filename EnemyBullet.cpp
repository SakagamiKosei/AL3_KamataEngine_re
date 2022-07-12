#include "EnemyBullet.h"
#include <cassert>

EnemyBullet::EnemyBullet()
{

}

EnemyBullet::~EnemyBullet()
{

}

void EnemyBullet::Initalize(Model* model, const Vector3& position,
	const Vector3& velocity)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("enemy_bullet.png");

	// ���[���h�g�����X�t�H�[���̏�����
	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	// �����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

	vectorChange_ = new VectorChange();

	// �����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;

	worldTransform_.scale_ = { 1,1,1 };

}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void EnemyBullet::Update()
{
	// ���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
	worldTransform_.translation_ += velocity_;
	vectorChange_->MyUpdate(worldTransform_);
	// ���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::OnCollision()
{
	isDead_ = false;
}

Vector3 EnemyBullet::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

Vector3 EnemyBullet::GetRadius()
{
	Vector3 enemyBulletRadius;
	// ���̑傫���𔼕��ɂ��Ĕ��a�ɂ���
	enemyBulletRadius.x = worldTransform_.scale_.x / 2;
	// �c�̑傫���𔼕��ɂ��Ĕ��a�ɂ���
	enemyBulletRadius.y = worldTransform_.scale_.y / 2;
	// ���s���̑傫���𔼕��ɂ��Ĕ��a�ɂ���
	enemyBulletRadius.z = worldTransform_.scale_.z / 2;

	return enemyBulletRadius;
}
