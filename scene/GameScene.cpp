#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <fstream>
using namespace std;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	//delete enemy_;
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
	delete debugCamera_;
	delete skydome_;
	delete modelSkydome_;
	delete railCamera_;
	// bullet_の解放
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		delete enemyBullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("kamata.ico");

	//レティクルのテクスチャ
	TextureManager::Load("reticle.png");
	
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 600;
	viewProjection_.Initialize();
	////モデル
	model_ = Model::Create();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	Vector3 playerPosition(0, 0, 50);
	player_->Initialize(model_, textureHandle_, playerPosition);


	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// 天球の生成
	skydome_ = new Skydome;
	// 天球の初期化
	skydome_->Initialize(modelSkydome_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//レールカメラの生成
	railCamera_ = new RailCamera();

	railCamera_->Initialize();

	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	LoadEnemyPopData();
}

void GameScene::Update() {
	

	// レールカメラの生成
	railCamera_->Update();

	// デバッグカメラ
	debugCamera_->Update();

#ifdef _DEBUG
	
	if (input_->TriggerKey(DIK_C) && isDebugCameraActive_ == false) {
		isDebugCameraActive_ = true;
	} 

	if (input_->TriggerKey(DIK_V) && isDebugCameraActive_ == true) {
		isDebugCameraActive_ = false;
	} 
#endif // _DEBUG

	// カメラの処理
	if (isDebugCameraActive_ == true) {
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}


	// 自キャラの更新
	player_->Update(viewProjection_);

	// 敵キャラの更新
	UpdateEnemyPopCommands();

	// 敵キャラの更新
	//enemy_->Update();
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}

	// 弾更新
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	// 天球の更新
	skydome_->Update();

	// 衝突判定と応答
	CheckAllCollisions();

	// デスフラグの立った弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 自キャラの描画
	player_->Draw(viewProjection_);

	// 敵キャラの描画
	//enemy_->Draw(viewProjection_);
	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}

	// 弾の描画
	for (EnemyBullet* bullet : enemyBullets_) {
	    bullet->Draw(viewProjection_);
	}

	// 天球の描画
	skydome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	player_->DrawUI();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	//リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::EnemySpawn(Vector3 position, Vector3 velocity) {
	Enemy* enemy = new Enemy();
	// 初期化
	enemy->Initialize(model_, position, velocity);
	// 敵キャラに自キャラのアドレスを渡す
	enemy->SetPlayer(player_);
	//敵キャラにゲームシーンを渡す
	enemy->SetGameScene(this);
	//リストに登録
	enemys_.push_back(enemy);
}

void GameScene::LoadEnemyPopData() {
	//ファイルを開く
	std::ifstream file;
	file.open("enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {

	//待機処理
	if (enemyPopWaitFlag) {
		enemyPopWaitTimer--;
		if (enemyPopWaitTimer <= 0) {
		    //待機完了
			enemyPopWaitFlag = false;
		}
		return;
	}
	
	//1行分の文字列を入れる変数
	std::string line;

	///コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
	    //1行文の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');
		
		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
		    //コメント行を飛ばす
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0) {
		    //x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			//y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			//z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//敵を発生させる
			EnemySpawn(Vector3(x, y, z), {0.2f, 0.2f, 0.2f});
		}
		//WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			enemyPopWaitFlag = true;
			enemyPopWaitTimer = waitTime;

			//コマンドループを抜ける
			break;
		}
	}
}
  
/// <summary>
/// 衝突判定と応答
/// </summary>
void GameScene::CheckAllCollisions() {
    //判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	#pragma region 自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets_) {
	    //敵弾の座標
		posB = bullet->GetWorldPosition();

		float radius1 = 1;
	    float radius2 = 1;

		float hit = (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
		            (posB.z - posA.z) * (posB.z - posA.z);

		float radius = (radius1 + radius2) * (radius1 + radius2);
		
		//球と球の交差判定
		if (hit <= radius)
		{
		    //自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
	#pragma endregion

	#pragma region 自弾と敵キャラの当たり判定
	// 敵キャラの座標
	for (Enemy* enemy : enemys_) {
		posA = enemy->GetWorldPosition();

		// 敵キャラと自弾全ての当たり判定
		for (PlayerBullet* bullet : playerBullets) {
			// 自弾の座標
			posB = bullet->GetWorldPosition();

			float radius1 = 2;
			float radius2 = 1;

			float hit = (posB.x - posA.x) * (posB.x - posA.x) +
			            (posB.y - posA.y) * (posB.y - posA.y) +
			            (posB.z - posA.z) * (posB.z - posA.z);

			float radius = (radius1 + radius2) * (radius1 + radius2);

			// 球と球の交差判定
			if (hit <= radius) {
				// 敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
		
				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}
    #pragma endregion

	#pragma region 自弾と敵弾の当たり判定
	for (PlayerBullet* playerBullet : playerBullets) {
		posA = playerBullet->GetWorldPosition();
		for (EnemyBullet* enemyBullet : enemyBullets_) {
			// 敵弾の座標
			posB = enemyBullet->GetWorldPosition();

			float radius1 = 1;
			float radius2 = 1;

			float hit = (posB.x - posA.x) * (posB.x - posA.x) +
			            (posB.y - posA.y) * (posB.y - posA.y) +
			            (posB.z - posA.z) * (posB.z - posA.z);

			float radius = (radius1 + radius2) * (radius1 + radius2);

			// 球と球の交差判定
			if (hit <= radius) {
				// 敵弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
				//自弾
				playerBullet->OnCollision();
			}
		}
	}
	
    #pragma endregion
}
