#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include "VectorChange.h"

/// <summary>
/// ���L�����̒e
/// </summary>
class PlayerBullet 
{
public:
	PlayerBullet();
	~PlayerBullet();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	void Initalize(Model* model, const Vector3& position);

	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// ���f���̃|�C���^(�؂�Ă�����)
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	VectorChange* vectorChange_ = nullptr;
};