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


}

void Enemy::Update() {
	// ワールド行列の更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
