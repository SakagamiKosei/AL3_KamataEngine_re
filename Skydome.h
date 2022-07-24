#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "VectorChange.h"

/// <summary>
/// �V��
/// </summary>
class Skydome
{
private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// ���f��
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	VectorChange* vectorChange_ = nullptr;

public:
	Skydome();
	~Skydome();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model);



	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewProjection);
};