#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include "VectorChange.h"
#include "DebugText.h"

/// <summary>
/// ���L�����̒e
/// </summary>
class PlayerBullet 
{
public:
	PlayerBullet();
	~PlayerBullet();
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	void Initalize(Model*model,const Vector3& position,const Vector3& velocity);
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	bool IsDead()const { return isDead_; }

	/// <summary>
	/// �Փ˂����o������Ăяo�����R�[���o�b�N�֐�	
	/// </summary>
	void OnCollision();

private:
	// ���[���h�ϊ��f�[�^
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