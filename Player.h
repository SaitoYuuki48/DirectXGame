#pragma once

#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "PlayerBullet.h"
#include <list>

class Player {

public:
	~Player();

	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection& viewProjection);

	//弾
	std::list<PlayerBullet*> bullets_;

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 弾リストを追加
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

private :
	void Rotate();

	void Attack();

private:
	Input* input_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//弾
	//PlayerBullet* bullet_ = nullptr;
};