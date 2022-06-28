#pragma once
#include "Model.h"
#include "WorldTransform.h"	
#include "DebugText.h"
#include "Input.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "TextureManager.h"
#include "VectorChange.h"
#include "MyMath.h"
#include "PlayerBullet.h"
#include <memory.h>
#include <list>

class Player {

public:
	Player();
	~Player();
/// <summary>
///  ������
/// </summary>
	void Initialize(Model* model,uint32_t textureHandle);
/// <summary>
///  �X�V
/// </summary>
    void Rotate();
	void Update();

/// <summary>
/// �U��
/// </summary>
	void Attack();

/// <summary>
///  �`��
/// </summary>
	void Draw(ViewProjection &viewProjection_);

private:
	// ���[���h�ϊ��s��f�[�^
	WorldTransform worldTransform_;
	// ���f��
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	// ���͏������邽��
	Input* input_ = nullptr;
	// �f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;
	// �x�N�g���ϊ�
	VectorChange* vectorChange_ = nullptr;
	MyMath* myMath_ = nullptr;

	// �e
	std::list<std::unique_ptr<PlayerBullet>>bullets_;
};

