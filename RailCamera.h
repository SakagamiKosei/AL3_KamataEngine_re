#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DebugText.h"
#include "VectorChange.h"

/// <summary>
/// ���[���J����
/// </summary>
class RailCamera {
public:
	RailCamera();
	~RailCamera();
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="position">�������W</param>
	/// <param name="rotation">�����p�x</param>
	void Initialize(const Vector3& position, const Vector3& rotation);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	//Vector3 GetWorldPosition();
	/// <summary>
	/// �r���[�v���W�F�N�V�������擾
	/// </summary>
	/// <returns>�r���[�v���W�F�N�V����</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	//���[���h�s��̎擾
	const WorldTransform& GetWorldMatrix()const { return worldTransform_; }


private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	//�f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;

	VectorChange* vectorChange_ = nullptr;
};