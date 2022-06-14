#include "Player.h"
#include <cassert>


Player::Player()
{

}
Player::~Player()
{
	delete vectorChange_;
	delete myMath_;
}

/// <summary>
/// 
/// </summary>
/// <param name="model"></param>
/// <param name="textureHandle"></param>
void Player::Initialize(Model* model, uint32_t textureHandle)
{
    // NULL�|�C���^�`�F�b�N
	assert(model);

	// �����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	// 3D���f���̐���
	model_ = model;
	// �t�@�C�������w�肵�ăe�N�X�`����ǂݍ���
	textureHandle_ = TextureManager::Load("player_shade.jpg");

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//vectorChange_ = new VectorChange();

	myMath_ = new MyMath();

}

void Player::Rotate()
{
	// ��]����
	{
		// �L�����N�^�[�̉�]�x�N�g��
		Vector3 roteMove = { 0,0,0 };

		const float kChestRotSpeed = 0.02f;
		// �����������ňړ��x�N�g����ύX
		if (input_->PushKey(DIK_LEFT)) 
		{
			worldTransform_.rotation_.y += kChestRotSpeed;
		}
		else if (input_->PushKey(DIK_RIGHT)) 
		{
			worldTransform_.rotation_.y -= kChestRotSpeed;
		}

		debugText_->SetPos(50, 70);
		debugText_->Printf("PlayerRot:(%f,%f,%f)", worldTransform_.rotation_.x,
			worldTransform_.rotation_.y, worldTransform_.rotation_.z);

		// ��]���E���W
		/*const float kRoteLimitX = 45;*/
		const float kRoteLimitY = 45.0f;


		//// ���W�ړ�(�x�N�g���̉��Z)
		//worldTransform_.rotation_ += roteMove;


		// �͈͂𒴂��Ȃ�����
		/*worldTransform_.rotation_.x = myMath_->MinNum(worldTransform_.rotation_.x, +kRoteLimitX);
		worldTransform_.rotation_.x = myMath_->MaxNum(worldTransform_.rotation_.x, -kRoteLimitX);*/


		worldTransform_.rotation_.y = myMath_->Clamp(myMath_->ConvertToRadian(-kRoteLimitY), myMath_->ConvertToRadian(kRoteLimitY), worldTransform_.rotation_.y);

		//worldTransform_.translation_.y = myMath_->MinNum(worldTransform_.translation_.y, +kMoveLimitY);
		//worldTransform_.translation_.y = myMath_->MaxNum(worldTransform_.translation_.y, -kMoveLimitY);
	}
}

void Player::Attack()
{
	if (input_->PushKey(DIK_SPACE))
	{
		// �e�𐶐����A������
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initalize(model_, worldTransform_.translation_);

		// �e��o�^����
		bullet_ = newBullet;
	}
}

void Player::Update()
{
	
	// �L�����N�^�[���񏈗�
	// ����(��])
	Rotate();

	// �ړ��x�N�g����ύX���鏈��

	// �L�����N�^�[�ړ�����
	{
		// �L�����N�^�[�̈ړ��x�N�g��
		Vector3 move = { 0,0,0 };

		// �L�����N�^�[�̈ړ�����
		const float kCharacterSpeed = 0.1f;

		// �����������ňړ��x�N�g����ύX
		// �����E�L�[�������Ă�����move(�ړ���)��ω�������
		if (input_->PushKey(DIK_LEFT))
		{
			move = { -kCharacterSpeed,0,0 };
		}
		else if (input_->PushKey(DIK_RIGHT))
		{
			move = { kCharacterSpeed,0,0 };
		}
		else if (input_->PushKey(DIK_UP))
		{
			move = { 0,kCharacterSpeed,0 };
		}
		else if (input_->PushKey(DIK_DOWN))
		{
			move = { 0,-kCharacterSpeed,0 };
		}




		// �ړ����E���W
		const float kMoveLimitX = 35;
		const float kMoveLimitY = 20;


		// ���W�ړ�(�x�N�g���̉��Z)
		worldTransform_.translation_ += move;


		// �͈͂𒴂��Ȃ�����
	
		worldTransform_.translation_.x = myMath_->Clamp(-kMoveLimitX, kMoveLimitX,worldTransform_.translation_.x);

	}

	// �L�����N�^�[�U������
	Attack();

	if (bullet_)
	{
		bullet_->Update();
	}
	vectorChange_->MyUpdate(worldTransform_);

	
	// �s��X�V
	// �s��̓]��
	worldTransform_.TransferMatrix();

	 //�L�����N�^�[�̍��W����ʕ\�����鏈��
	debugText_->SetPos(50, 50);
	debugText_->Printf("PlayerPos:(%f,%f,%f)", worldTransform_.translation_.x,
		worldTransform_.translation_.y, worldTransform_.translation_.z);

}




void Player::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	// �e�`��
	if (bullet_) {
		bullet_->Draw(viewProjection_);
	}
}

