#include "Player.h"
#include <cassert>
#include <Model.h>
#include <WorldTransform.h>
#include "ImGuiManager.h"

#include "Affine.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	input_ = Input::GetInstance();

	// ��������󂯎�������f�����ǂݍ��܂�Ă��邩�`�F�b�N
	assert(model);
	// �������烂�f���ƃe�N�X�`���n���h�����󂯎��
	model_ = model;
	textureHandle_ = textureHandle;
	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	// x,y,z�����̃X�P�[�����O��ݒ�
	worldTransform_.scale_ = {5.0f, 1.0f, 1.0f};
	// x,y,z�����̉�]��ݒ�
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	// x,y,z�̕����̃X�P�[�����O��ݒ�
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
}

void Player::Update() {
	// �L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = {0, 0, 0};

	// �L�����N�^�[�̈ړ�����
	const float kCharacterSpeed = 0.2f;

	// �����������ňړ��x�N�g����ύX(���E)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// �����������ňړ��x�N�g����ύX(�㉺)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	// �ړ����E���W
	const float kMoveLimitX = 30;
	const float kMoveLimitY = 20;

	// �͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// ���W�ړ�(�x�N�g���̉��Z)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	// �s��̓]���@�s��̌v�Z��ɍs��
	worldTransform_.TransferMatrix();

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// �L�����N�^�[�̍��W����ʕ\�����鏈��
	ImGui::Begin("Debug");
	float playerPos[] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("PlayerPos", playerPos, 0.0f, 128.0f);
	ImGui::End();

	worldTransform_.translation_.x = playerPos[0];
	worldTransform_.translation_.y = playerPos[1];
	worldTransform_.translation_.z = playerPos[2];
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}