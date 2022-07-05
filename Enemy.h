#pragma once
#include "Model.h"
#include "DebugText.h"
#include "EnemyBullet.h"

// ���@�N���X�̑O���錾
class Player;


/// <summary>
/// �G
/// </summary>
// �s���t�F�[�Y
enum class Phase {
	Approach, // �ڋ߂���
	Leave, // ���E����
};

class Enemy
{
public:
	Enemy();
	~Enemy();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model"></param>
	/// <param name="textureHandle"></param>
	void Initialize(Model* model, uint32_t textureHandle);
	/// <summary>
	/// �ڋ�
	/// </summary>
	void ApproachMove();
	/// <summary>
	/// �����
	/// </summary>
	void LeaveMove();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection_"></param>
	void Draw(ViewProjection& viewProjection_);
	/// <summary>
	///	�e����
	/// </summary>
	void Fire();
/// <summary>
/// �ڋ߃t�F�[�Y������
	void ApproachInitialize();
/// </summary>

/// <summary>
/// �ڋ߃t�F�[�Y�̍X�V�֐�
/// </summary>
	void ApproachUpdate();

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

public:
	// ���ˊԊu
	static const int kFireInterval = 60;

private:
	// ���[���h�g�����X�t�H�[��
	WorldTransform worldTransform_;
	// ���f��
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	// �f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;
	// �x�N�g���ϊ�
	VectorChange* vectorChange_ = nullptr;
	// �e
	std::list<std::unique_ptr<EnemyBullet>>bullets_;
	// �����t�F�[�Y
	Phase phase_ = Phase::Approach;
	// ���˃^�C�}�[
	int32_t fireTimer_ = 0;
	// ���L����
	Player* player_ = nullptr;
};