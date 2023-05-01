#include "Player.h"
#include <cassert>
#include <Model.h>
#include <WorldTransform.h>

void Player::Initialize() 
{
	input_ = Input::GetInstance();
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
	worldTransform_.translation_ += move;


}

void Player::Draw() {}
