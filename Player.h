#pragma once

#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "PlayerBullet.h"
#include <list>
#include <Sprite.h>

class Player {

public:
	Player();

	~Player();

	void Initialize(Model* model, uint32_t textureHandle, const Vector3& position);

	void Update(ViewProjection& viewProjection);

	void Draw(ViewProjection& viewProjection);

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	void SetParent(const WorldTransform* parent);

   /// <summary>
   /// UI描画 
   /// </summary>
	void DrawUI();

private :
	void Rotate();

	void Attack();

public:
	// 弾
	std::list<PlayerBullet*> bullets_;

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 弾リストを追加
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }


private:
	Input* input_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
    //自機をカメラからずらす
	Vector3 position_;

	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	//2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;

	//弾
	//PlayerBullet* bullet_ = nullptr;
};