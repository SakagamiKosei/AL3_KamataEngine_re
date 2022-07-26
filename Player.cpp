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
	this->model_ = model;
	// �t�@�C�������w�肵�ăe�N�X�`����ǂݍ���
	textureHandle_ = TextureManager::Load("player_shade.jpg");

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	myMath_ = new MyMath();

	worldTransform_.scale_ = { 1,1,1 };

}

void Player::Rotate()
{
	// ��]����
	{
		// �L�����N�^�[�̉�]�x�N�g��
		Vector3 roteMove = { 0,0,0 };

		const float kChestRotSpeed = 0.02f;
		// �����������ňړ��x�N�g����ύX
		if (input_->PushKey(DIK_I)) 
		{
			worldTransform_.rotation_.y += kChestRotSpeed;
		}
		else if (input_->PushKey(DIK_U)) 
		{
			worldTransform_.rotation_.y -= kChestRotSpeed;
		}

		// �L�����N�^�[�̉�]�p�x����ʕ\�����鏈��
		debugText_->SetPos(50, 70);
		debugText_->Printf("PlayerRot:(%f)", 
			worldTransform_.rotation_.y);

		// ��]���E���W
		const float kRoteLimitY = 45.0f;


		//// ���W�ړ�(�x�N�g���̉��Z)
		

		// �͈͂𒴂��Ȃ�����

	/*	worldTransform_.rotation_.y = myMath_->Clamp(myMath_->ConvertToRadian(-kRoteLimitY), myMath_->ConvertToRadian(kRoteLimitY), worldTransform_.rotation_.y);*/

	}
}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		// �e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);


		// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = MathUtility::Vector3TransformNormal(velocity, worldTransform_.matWorld_);

		// ���L�����̍��W���R�s�[
		Vector3 position = worldTransform_.translation_;

		// �e�𐶐����A������
		std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initalize(model_,position,velocity);

		// �e��o�^����
		playerBullets_.push_back(std::move(newBullet));

	}


}

void Player::Update()
{
	// �f�X�t���O�̗������e���폜
	playerBullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
		return bullet->IsDead();
		});
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
		const float kChestRotSpeed = 0.001f;

		// �����������ňړ��x�N�g����ύX
		// �����E�L�[�������Ă�����move(�ړ���)��ω�������
		if (input_->PushKey(DIK_LEFT))
		{
			move = { -kCharacterSpeed,0,0 };
			worldTransform_.rotation_.y += kChestRotSpeed;
		}
		else if (input_->PushKey(DIK_RIGHT))
		{
			move = { kCharacterSpeed,0,0 };
			worldTransform_.rotation_.y -= kChestRotSpeed;
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
		// ��]���E
		const float kRotLimitY = 0.5f;
		// ���W�ړ�(�x�N�g���̉��Z)
		worldTransform_.translation_ += move;
	

		// �͈͂𒴂��Ȃ�����
		worldTransform_.translation_.x = myMath_->Clamp(-kMoveLimitX, kMoveLimitX,worldTransform_.translation_.x);
		worldTransform_.rotation_.y = myMath_->Clamp(-kRotLimitY, kRotLimitY, worldTransform_.rotation_.y);
	}

	// �L�����N�^�[�U������
	Attack();
	

	// �e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullets_)
	{
		bullet->Update();
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
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullets_)
	{
		bullet->Draw(viewProjection_);
	}
}

Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Player::GetRadius()
{
	Vector3 playerRadius;
	// ���̑傫���𔼕��ɂ��Ĕ��a�ɂ���
	playerRadius.x = worldTransform_.scale_.x / 2;
	// �c�̑傫���𔼕��ɂ��Ĕ��a�ɂ���
	playerRadius.y = worldTransform_.scale_.y / 2;
	// ���s���̑傫���𔼕��ɂ��Ĕ��a�ɂ���
	playerRadius.z = worldTransform_.scale_.z / 2;

	return playerRadius;
}

void Player::OnCollision()
{
	// �������Ȃ�
}

Vector3 transform(const Vector3& velocity, const Matrix4& matWorld)
{
	Vector3 result;
	
		result.x = velocity.x * matWorld.m[0][0] + velocity.y * matWorld.m[1][0] + velocity.z * matWorld.m[2][0] + matWorld.m[3][0];
	    result.y = velocity.x * matWorld.m[0][1] + velocity.y * matWorld.m[1][1] + velocity.z * matWorld.m[2][1] + matWorld.m[3][1];
		result.z = velocity.x * matWorld.m[0][2] + velocity.y * matWorld.m[1][2] + velocity.z * matWorld.m[2][2] + matWorld.m[3][2];
	
	return result;
}