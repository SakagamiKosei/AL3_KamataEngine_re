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
	worldTransform_.scale_ = { 1,1,1, };
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

void Enemy::Update()
{
	switch (phase_) {
	case Phase::Approach:
	default:
		ApproachMove();
		break;
	case Phase::Leave:
		LeaveMove();
		break;
	}
}

void Enemy::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
