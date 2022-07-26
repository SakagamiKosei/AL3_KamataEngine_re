#include "RailCamera.h"


RailCamera::RailCamera() {

}


RailCamera::~RailCamera()
{

}


void RailCamera::Initialize(const Vector3& position, const Vector3& rotation)
{
	//���[���h�g�����X�t�H�[���̏����ݒ�
	// �����Ń��[���h���W���󂯎���ă��[���h�g�����X�t�H�[���ɐݒ�
	worldTransform_.translation_ = position;
	// �����ŉ�]�p[���W�A��]���󂯂Ƃ��ă��[���h�g�����X�t�H�[���ɐݒ�
	/*worldTransform_.rotation_ = rotation*/

	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
	worldTransform_.Initialize();

}

void RailCamera::Update()
{
	//���[���h�g�����X�t�H�[���̍��W�̐��l�����Z�����肷��(�ړ�)
	worldTransform_.translation_ += Vector3(0, 0, 0.1f);
	//���[���h�g�����X�t�H�[���̊p�x�̐��l�����Z�����肷��(��])
   /* worldTransform_.rotation_+=Vector3(0,0.1f,0);*/

	vectorChange_->MyUpdate(worldTransform_);
	//���[���h�g�����X�t�H�[���̃��[���h�s��̍Čv�Z
	//viewProjection_.UpdateMatrix();
	//���[���h�s��̕��s�ړ��������擾
	viewProjection_.eye.x = worldTransform_.matWorld_.m[3][0];
	viewProjection_.eye.y = worldTransform_.matWorld_.m[3][1];
	viewProjection_.eye.z = worldTransform_.matWorld_.m[3][2];

	// ���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	// ���[���J�����̉�]�𔽉f
	//forward = worldTransform_.matWorld_.transform(forward, worldTransform_.matWorld_);
	forward = MathUtility::Vector3TransformNormal(forward, worldTransform_.matWorld_);
	// ���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	viewProjection_.target = viewProjection_.eye + forward;
	// ���[���h����x�N�g��
	Vector3 up(0, 1, 0);
	// ���[���J�����̉�]�𔽉f(���[���J�����̏���x�N�g��)
	//viewProjection_.up = worldTransform_.matWorld_.transform(up, worldTransform_.matWorld_);
	viewProjection_.up = MathUtility::Vector3TransformNormal(up, worldTransform_.matWorld_);
	//�r���[�v���W�F�N�V�����̍X�V�ƒ萔�o�b�t�@�ւ̓]��
	viewProjection_.UpdateMatrix();


	DebugText* debugText_ = DebugText::GetInstance();

	// �f�o�b�N�p�\��
	debugText_->SetPos(70, 300);
	debugText_->Printf("RailCamera Pos:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	debugText_->SetPos(70, 320);
	debugText_->Printf("Target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

}

//Vector3 RailCamera::GetWorldPosition()
//{
//	Vector3 worldPos;
//
//	worldPos.x = worldTransform_.translation_.x;
//	worldPos.y = worldTransform_.translation_.y;
//	worldPos.z = worldTransform_.translation_.z;
//
//	return worldPos;
//
//}
//
