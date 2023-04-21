#include "Player.h"
#include "cassert"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//引数から受け取ったモデルが読み込まれているかチェック
	assert(model);
	//引数からモデルとテクスチャハンドルを受け取る
	model_ = model;
	textureHandle_ = textureHandle;

	//ワールドトランスフォーム初期化
	worldTransform_.Initialize();
}

void Player::Update() { 
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
