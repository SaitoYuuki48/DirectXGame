#include "Skydome.h"

void Skydome::Initialize(Model* model) {
	// 引数から受け取ったモデルが読み込まれているかチェック
	assert(model);
	// 引数からモデルとテクスチャハンドルを受け取る
	model_ = model;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// x,y,z方向のスケーリングを設定
	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};
	// x,y,z方向の回転を設定
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	// x,y,zの方向のを設定
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
}

void Skydome::Update() {
	worldTransform_.UpdateMatrix();

	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.0002f;

	// WorldTransformのY軸まわり角度を回転速さ分減算する
	worldTransform_.rotation_.y -= kRotSpeed;

	worldTransform_.rotation_.z -= kRotSpeed;
}

void Skydome::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
