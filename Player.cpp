#include "Player.h"
#include "cassert"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//��������󂯎�������f�����ǂݍ��܂�Ă��邩�`�F�b�N
	assert(model);
	//�������烂�f���ƃe�N�X�`���n���h�����󂯎��
	model_ = model;
	textureHandle_ = textureHandle;

	//���[���h�g�����X�t�H�[��������
	worldTransform_.Initialize();
}

void Player::Update() { 
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
