#pragma once

#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include <list>

// 自機クラスの前方宣言
class Player;

// GameSceneの前方宣言(苦肉の策)
class GameScene;

class Enemy {
public:

	Enemy();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, Model* modelExplosion, const Vector3& position, const Vector3& velocity);

	///< summary>
	/// 更新
	///  </summary>
	void Update();

	///< summary>
	/// 描画
	///  </summary>
	/// <param name="viewPrejection& viewProjection
	void Draw(const ViewProjection& viewProjection);

	void DrawExplosion(const ViewProjection& viewProjection);

	//接近フェーズの初期化
	void ApproachInit();

	void SetPlayer(Player* player) { player_ = player; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//ゲームシーンを敵に渡す
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:
	// 接近する
	void Approach();
	// 離脱する
	void Leave();

	///< summary>
	/// 弾発射
	///  </summary>
	void Fire();

public:
	// 発射間隔
	static const int kFireInterval = 60;

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	bool IsDead() const { return isDead_; }

private: // 変数
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 velocity_;

	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};

	//フェーズ
	Phase phase_ = Phase::Approach;

	// 弾
	EnemyBullet* bullet_ = nullptr;

	//発射タイマー
	int32_t fireTimer_ = 0;

	//自キャラ
	Player* player_;

	// ゲームシーン
	GameScene* gameScene_;

	// デスフラグ
	bool isDead_ = false;

	Model* modelExplosion_ = nullptr;
};
