#include "Player.h"
#include <cassert>
#include <Model.h>
#include <WorldTransform.h>
#include "Affine.h"


void Player::Initialize() 
{
	input_ = Input::GetInstance();

	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	//x,y,z�����̃X�P�[�����O��ݒ�
	worldTransform_.scale_ = {5.0f, 1.0f, 1.0f};
	//x,y,z�����̉�]��ݒ�
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	//x,y,z�̕����̃X�P�[�����O��ݒ�
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
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


	////�X�P�[�����O�s���錾
	//Matrix4x4 matScale;
	//
	//matScale.m[0][0] = worldTransform_.scale_.x;
	//matScale.m[1][1] = worldTransform_.scale_.y;
	//matScale.m[2][2] = worldTransform_.scale_.z;
	//matScale.m[3][3] = 1;

	////X����]�s��
	//Matrix4x4 matRotX;

	//matRotX.m[0][0] = 1;
	//matRotX.m[1][1] = cosf(worldTransform_.rotation_.x);
	//matRotX.m[1][2] = sinf(worldTransform_.rotation_.x);
	//matRotX.m[2][1] = -sinf(worldTransform_.rotation_.x);
	//matRotX.m[2][2] = cosf(worldTransform_.rotation_.x);
	//matRotX.m[3][3] = 1;

	////Y����]�s��
	//Matrix4x4 matRotY;

	//matRotY.m[0][0] = cosf(worldTransform_.rotation_.y);
	//matRotY.m[0][2] = -sinf(worldTransform_.rotation_.y);
	//matRotY.m[1][1] = 1;
	//matRotY.m[2][0] = sinf(worldTransform_.rotation_.y);
	//matRotY.m[2][2] = cosf(worldTransform_.rotation_.y);
	//matRotY.m[3][3] = 1;

	//// Z����]�s��
	//Matrix4x4 matRotZ;
	//matRotZ.m[0][0] = cosf(worldTransform_.rotation_.z);
	//matRotZ.m[0][1] = -sinf(worldTransform_.rotation_.z);
	//matRotZ.m[1][0] = sinf(worldTransform_.rotation_.z);
	//matRotZ.m[1][1] = cosf(worldTransform_.rotation_.z);
	//matRotZ.m[2][2] = 1;
	//matRotZ.m[3][3] = 1;

	//Matrix4x4 matRot = Multiply(matRotX, Multiply(matRotY, matRotZ));

	////���s�ړ��s���錾
	//Matrix4x4 matTrans;

	//matTrans.m[0][0] = 1;
	//matTrans.m[1][1] = 1;
	//matTrans.m[2][2] = 1;
	//matTrans.m[3][0] = worldTransform_.translation_.x;
	//matTrans.m[3][1] = worldTransform_.translation_.y;
	//matTrans.m[3][2] = worldTransform_.translation_.z;
	//matTrans.m[3][3] = 1;

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_,worldTransform_.rotation_,worldTransform_.translation_);

	//�s��̓]���@�s��̌v�Z��ɍs��
	worldTransform_.TransferMatrix();

}

void Player::Draw() {}
