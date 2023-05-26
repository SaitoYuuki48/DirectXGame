#include "PlayerBullet.h"
#include <assert.h>

void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	//NULL�|�C���^�`�F�b�N
	assert(model);

    model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("uvChecker.png");

	//���[���h�g�����X�t�H�[���̏�����
	//�����Ŏ󂯎�����������ʂ��Z�b�g
	worldTransform_.translation_ = ;
}

void PlayerBullet::Update() {}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
