#include "Skydome.h"
#include <cassert>

Skydome::Skydome()
{

}

Skydome::~Skydome()
{

}


void Skydome::Initialize(Model* model)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	// �����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	// 3D���f���̐���
	this->model_ = model;

	worldTransform_.translation_ = { 0,0,0 };
	worldTransform_.scale_ = { 500.0f,500.0f,500.0f };

	// ���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	vectorChange_ = new VectorChange();
}

void Skydome::Update()
{
	vectorChange_->MyUpdate(worldTransform_);
}

void Skydome::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_);
}