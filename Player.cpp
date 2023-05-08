#include "Player.h"
#include <cassert>
#include <Model.h>
#include <WorldTransform.h>

void Player::Initialize() 
{
	input_ = Input::GetInstance();

	//x,y,z�����̃X�P�[�����O��ݒ�
	worldTransform_.scale_ = {5.0f, 1.0f, 1.0f};


}

void Player::Update() 
{
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = {0, 0, 0};

	//�L�����N�^�[�̈ړ�����
	const float kCharacterSpeed = 0.2f;

	//�����������ňړ��x�N�g����ύX(���E)
	if (input_->PushKey(DIK_LEFT))
	{
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	//�����������ňړ��x�N�g����ύX(�㉺)
	if (input_->PushKey(DIK_UP))
	{
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y += kCharacterSpeed;
	}

	//���W�ړ�(�x�N�g���̉��Z)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	//�X�P�[�����O�s���錾
	Matrix4x4 matScale;
	
	matScale.m[0][0] = worldTransform_.scale_.x;
	matScale.m[1][1] = worldTransform_.scale_.y;
	matScale.m[2][2] = worldTransform_.scale_.z;
	matScale.m[3][3] = 1;

	//�s��̓]���@�s��̌v�Z��ɍs��
	worldTransform_.TransferMatrix();


}

void Player::Draw() {}
