#pragma once

#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"

class PlayerBullet {
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	void Initialize(Model* model, const Vector3& position);

	///<summary>
	///�X�V
	/// </summary>
	void Update();

	///< summary>
	/// �`��
	///  </summary>
	/// <param name="viewPrejection& viewProjection
	void Draw(const ViewProjection& viewProjection);

private:
	//Input* input_ = nullptr;

	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// ���f��
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
};
