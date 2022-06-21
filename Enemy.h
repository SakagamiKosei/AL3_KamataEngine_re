#pragma once
#include <Model.h>
#include "VectorChange.h"
#include "DebugText.h"
/// <summary>
/// �G
/// </summary>
class Enemy
{
public:
	Enemy();
	~Enemy();

	void Initialize(Model* model, uint32_t textureHandle);
	void Move();
	void Update();
	void Draw(ViewProjection& viewProjection_);
private:
	// ���[���h�g�����X�t�H�[��
	WorldTransform worldTransform_;
	// ���f��
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	// �f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;

	VectorChange* vectorChange_ = nullptr;
};