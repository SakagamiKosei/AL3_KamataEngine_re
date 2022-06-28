#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include "VectorChange.h"
#include "DebugText.h"

/// <summary>
/// �G�L�����̒e
/// </summary>
class EnemyBullet
{
public:
	EnemyBullet();
	~EnemyBullet();
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	void Initalize(Model* model, const Vector3& position, const Vector3& velocity);
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <returns></returns>
	bool IsDead()const { return isDead_; }
private:
	// ���[���h�g�����X�t�H�[��
	WorldTransform worldTransform_;
	// ���f���̃|�C���^(�؂�Ă�����)
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	// �x�N�g���ϊ�
	VectorChange* vectorChange_ = nullptr;
	// ���x
	Vector3 velocity_;
	// �f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;

	// ����<frm>
	static const int32_t kLifeTime = 60 * 5;

	// �f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	// �f�X�t���O
	bool isDead_ = false;

};