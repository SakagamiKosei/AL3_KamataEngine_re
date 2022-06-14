#include "PlayerBullet.h"
#include <cassert>


PlayerBullet::PlayerBullet()
{

}

PlayerBullet::~PlayerBullet()
{

}

void PlayerBullet::Initalize(Model* model, const Vector3& position)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");

	// ���[���h�g�����X�t�H�[���̏�����
	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	// �����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

	vectorChange_ = new VectorChange();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerBullet::Update()
{
	vectorChange_->MyUpdate(worldTransform_);
}