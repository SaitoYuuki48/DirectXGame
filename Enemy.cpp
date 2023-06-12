#include "Enemy.h"
#include <assert.h>

void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// 引数から受け取ったモデルが読み込まれているかチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("debugfont.png");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	// x,y,z方向のスケーリングを設定
	worldTransform_.scale_ = {2.0f, 2.0f, 1.0f};
}

void Enemy::Update() {
	// ワールド行列の更新
	worldTransform_.UpdateMatrix();

	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	/*worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z -= velocity_.z;*/

	switch (phase_) {
	case Phase::Approach: // 接近
		Approach();
		break;
	case Phase::Leave: // 離脱
		Leave();
		break;
	}
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::Approach() {
	// 移動(ベクトルを加算)
	worldTransform_.translation_.z -= velocity_.z;
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave() {
	// 移動(ベクトルを加算)
	worldTransform_.translation_.x -= velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
}
