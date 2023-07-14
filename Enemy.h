#pragma once

#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include <list>

// 自機クラスの前方宣言
class Player;

class Enemy {
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	///< summary>
	/// 更新
	///  </summary>
	void Update();

	///< summary>
	/// 描画
	///  </summary>
	/// <param name="viewPrejection& viewProjection
	void Draw(const ViewProjection& viewProjection);

	// 弾
	std::list<EnemyBullet*> bullets_;

	//接近フェーズの初期化
	void ApproachInit();

	void SetPlayer(Player* player) { player_ = player; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 弾リストを追加
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

public:
	//発射間隔
	static const int kFireInterval = 60;

private:
	// 接近する
	void Approach();
	// 離脱する
	void Leave();

	///< summary>
	/// 弾発射
	///  </summary>
	void Fire();

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
	Player* player_ = nullptr;

	//敵キャラ
	Enemy* enemy_ = nullptr;
};
