﻿#include "Player.h"
#include <cassert>
#include <Model.h>
#include <WorldTransform.h>
#include "ImGuiManager.h"

#include "Affine.h"


void Player::Initialize(Model* model, uint32_t textureHandle) 
{
	input_ = Input::GetInstance();

	// 引数から受け取ったモデルが読み込まれているかチェック
	assert(model);
	// 引数からモデルとテクスチャハンドルを受け取る
	model_ = model;
	textureHandle_ = textureHandle;
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//x,y,z方向のスケーリングを設定
	worldTransform_.scale_ = {5.0f, 1.0f, 1.0f};
	//x,y,z方向の回転を設定
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	//x,y,zの方向のスケーリングを設定
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
}


void Player::Update() 
{
	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT))
	{
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	//押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP))
	{
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	//移動限界座標
	const float kMoveLimitX = 30;
	const float kMoveLimitY = 20;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, + kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, - kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//座標移動(ベクトルの加算)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;


	// 行列の転送　行列の計算後に行う
	worldTransform_.TransferMatrix();

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_,worldTransform_.rotation_,worldTransform_.translation_);

	//キャラクターの座標を画面表示する処理
	ImGui::Begin("Debug");
	float playerPos[] = {worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z};
	ImGui::SliderFloat3("PlayerPos", playerPos, 0.0f, 128.0f);
	ImGui::End();

	worldTransform_.translation_.x = playerPos[0];
	worldTransform_.translation_.y = playerPos[1];
	worldTransform_.translation_.z = playerPos[2];


}

void Player::Draw(ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
