#include "Enemy.h"
#include <cassert>
Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

void Enemy::Initialize(Model* model, uint32_t textureHandle)
{


	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	// NULL�|�C���^�`�F�b�N
	assert(model);

	// �����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	// 3D���f���̐���
	this->model_ = model;


	// �t�@�C�������w�肵�ăe�N�X�`����ǂݍ���
	textureHandle_ = TextureManager::Load("enemy_sentai.png");

	debugText_ = DebugText::GetInstance();
	// �G�̏������W
	worldTransform_.translation_ = { 0,2,30 };
	// �G�̃X�P�[��
	worldTransform_.scale_ = { 1,1,1};
	//// �G����������Ɠ����ɒe�𔭎˂���
	//Fire();
	// �ڋ߃t�F�[�Y������
	ApproachInitialize();
}

void Enemy::ApproachMove()
{
	// �L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };

	// �L�����N�^�[�̈ړ�����
	const float kCharacterSpeed = 0.1f;

	move = { 0,0,-kCharacterSpeed };

	// ���W�ړ�(�x�N�g���̉��Z)
	worldTransform_.translation_ += move;

	vectorChange_->MyUpdate(worldTransform_);

	// �s��X�V
	// �s��̓]��
	worldTransform_.TransferMatrix();

	if (worldTransform_.translation_.z < 0.0f)
	{
		phase_ = Phase::Leave;
	}

	//�L�����N�^�[�̍��W����ʕ\�����鏈��
	debugText_->SetPos(50, 90);
	debugText_->Printf("EnemyPos:(%f,%f,%f)", worldTransform_.translation_.x,
		worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Enemy::LeaveMove()
{
	// �L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };

	// �L�����N�^�[�̈ړ�����
	const float kCharacterSpeed = 0.1f;

	move = { -kCharacterSpeed ,kCharacterSpeed,0 };

	// ���W�ړ�(�x�N�g���̉��Z)
	worldTransform_.translation_ += move;

	vectorChange_->MyUpdate(worldTransform_);

	// �s��X�V
	// �s��̓]��
	worldTransform_.TransferMatrix();

	//�L�����N�^�[�̍��W����ʕ\�����鏈��
	debugText_->SetPos(50, 90);
	debugText_->Printf("EnemyPos:(%f,%f,%f)", worldTransform_.translation_.x,
		worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Enemy::ApproachInitialize()
{
	// ���˃^�C�}�[��������
	fireTimer_ = kFireInterval;
}

void Enemy::ApproachUpdate()
{
	// ���˃^�C�}�[�J�E���g�_�E��
	fireTimer_--;
	// �w�莞�ԂɒB����
	if (fireTimer_ <= 3)
	{
		// �e�𔭎�
		Fire();
		// ���˃^�C�}�[��������
		fireTimer_ = kFireInterval;
	}

}

void Enemy::Update()
{
	// �f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		});
	switch (phase_) {
	case Phase::Approach:
	default:
		ApproachMove();
		break;
	case Phase::Leave:
		LeaveMove();
		break;
	}

	// �e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	ApproachUpdate();

	vectorChange_->MyUpdate(worldTransform_);


	// �s��X�V
	// �s��̓]��
	worldTransform_.TransferMatrix();

}

void Enemy::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	// �e�`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection_);
	}
}

void Enemy::Fire()
{
	// �e�̑��x
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, -kBulletSpeed);


	// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	velocity = MathUtility::Vector3TransformNormal(velocity, worldTransform_.matWorld_);

	// ���L�����̍��W���R�s�[
	Vector3 position = worldTransform_.translation_;

	// �e�𐶐����A������
	std::unique_ptr<EnemyBullet>newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initalize(model_, position, velocity);

	// �e��o�^����
	bullets_.push_back(std::move(newBullet));
}
