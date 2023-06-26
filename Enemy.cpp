#include "Enemy.h"
#include <assert.h>

Enemy::~Enemy() {
	// bullet_の解放
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

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

	ApproachInit();
}

void Enemy::Update() {
	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

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

	// 弾更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾の描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::ApproachInit() {
    //発射タイマーを初期化
	fireTimer_ = kFireInterval;
}

void Enemy::Approach() {
	// 移動(ベクトルを加算)
	worldTransform_.translation_.z -= velocity_.z;
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

	//発射タイマーカウントダウン
	//発射タイマーをデクリメント
	fireTimer_--;
	//指定時間に達した
	if (fireTimer_ == 30)
	{
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer_ = kFireInterval;
	}
}

void Enemy::Leave() {
	// 移動(ベクトルを加算)
	worldTransform_.translation_.x -= velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
}

void Enemy::Fire() {
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	bullets_.push_back(newBullet);
}
