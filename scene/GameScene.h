#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include <sstream>

#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public:  // メンバ関数
/// <summary>
/// 敵弾を追加する
/// </summary>
/// <param name="enemyBullet">敵弾</param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵の発生
	/// </summary>
	void EnemySpawn(Vector3 position, Vector3 velocity);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

   /// <summary>
   /// 敵発生コマンドの更新
   /// </summary>
	void UpdateEnemyPopCommands();

private: //関数

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

public:
	// 弾
	std::list<EnemyBullet*> enemyBullets_;

	// 敵リスト
	std::list<Enemy*> enemys_;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 3Dモデル
	Model* model_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 自キャラ
	Player* player_ = nullptr;

	// 自弾
	PlayerBullet* playerBullet_ = nullptr;

	// 敵キャラ
	Enemy* enemy_ = nullptr;

	// 敵弾
	EnemyBullet* enemyBullet_ = nullptr;

	// 天球
	Skydome* skydome_ = nullptr;

	// 3Dモデル
	Model* modelSkydome_ = nullptr;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	//敵の待機中のフラグ
	bool enemyPopWaitFlag = true;
	//待機タイマー
	float enemyPopWaitTimer = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
