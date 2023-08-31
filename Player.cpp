#include "Player.h"
#include <cassert>
#include <Model.h>
#include "ImGuiManager.h"

#include "Mt.h"

Player::Player() {}

Player::~Player() {
	//bullet_の解放
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}

	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle, const Vector3& position) {
	input_ = Input::GetInstance();

	// 引数から受け取ったモデルが読み込まれているかチェック
	assert(model);
	// 引数からモデルとテクスチャハンドルを受け取る
	model_ = model;
	textureHandle_ = textureHandle;
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// x,y,z方向のスケーリングを設定
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	// x,y,z方向の回転を設定
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	// x,y,zの方向のを設定
	//worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	// 自機をカメラからずらす
	worldTransform_.translation_ = position;

	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	//レティクル用テクスチャ
	uint32_t textureReticle = TextureManager::Load("reticle.png");

	//スプライト生成
	sprite2DReticle_ =
	    Sprite::Create(textureReticle, {640.0f, 360.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
}

void Player::Update(ViewProjection& viewProjection) {
	worldTransform_.UpdateMatrix();

	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// 旋回処理
	Rotate();

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	// 移動限界座標
	const float kMoveLimitX = 30;
	const float kMoveLimitY = 20;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	// ワールド行列の更新
	worldTransform_.UpdateMatrix();

#ifdef _DEBUG
	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Debug");
	float playerPos[] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("PlayerPos", playerPos, 0.0f, 128.0f);
	ImGui::End();

	worldTransform_.translation_.x = playerPos[0];
	worldTransform_.translation_.y = playerPos[1];
	worldTransform_.translation_.z = playerPos[2];
#endif // _DEBUG

	// 攻撃処理
	Attack();

	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	// 自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = {0, 0, 1.0f};
	// 自機からワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	// ベクトルの長さを整える
	offset = Multiply(kDistancePlayerTo3DReticle, Normalize(offset));
	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = Add(GetWorldPosition(), offset);
	worldTransform3DReticle_.UpdateMatrix();

	// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	Vector3 positionReticle = {
	    worldTransform3DReticle_.matWorld_.m[3][0], worldTransform3DReticle_.matWorld_.m[3][1],
	    worldTransform3DReticle_.matWorld_.m[3][2]
	};

	//ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0.0f, 0.0f, WinApp::kWindowWidth, WinApp::kWindowHeight, 0.0f, 1.0f);

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

	//ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = Transform(positionReticle, matViewProjectionViewport);
	
	//スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//model_->Draw(worldTransform3DReticle_, viewProjection);

	//弾の描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::DrawUI() { sprite2DReticle_->Draw(); }

void Player::Rotate() {
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		//WorldTransformのY軸まわり角度を回転速さ分減算する
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if(input_->PushKey(DIK_D)){
		//WorldTransformのY軸まわり角度を回転速さ分加算する
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		Vector3 playerWorldPosition = GetWorldPosition();

		//速度ベクトルを自機の向きに合わせて回転させる
		//velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		velocity = Subtract(
		    {worldTransform3DReticle_.matWorld_.m[3][0], worldTransform3DReticle_.matWorld_.m[3][1],
		     worldTransform3DReticle_.matWorld_.m[3][2]},
		    GetWorldPosition());
		velocity = Multiply(kBulletSpeed, Normalize(velocity));

		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, playerWorldPosition, velocity);

		//弾を登録する
		bullets_.push_back(newBullet);
	}

}

void Player::OnCollision() {}

void Player::SetParent(const WorldTransform* parent) {
    //親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

Vector3 Player::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}