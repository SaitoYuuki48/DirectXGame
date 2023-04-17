#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	//サウンド
	soundDataHandle_ = audio_->LoadWave("fanfare.wav");
	audio_->PlayWave(soundDataHandle_);
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);

	textureHandle_ = TextureManager::Load("debugfont.png");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	//モデル
	modelTextureHandle_ = TextureManager::Load("debugfont.png");
	model_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	//ライン
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() { 
	Vector2 position = sprite_->GetPosition(); 

	position.x += 2.0f;
	position.y += 1.0f;

	sprite_->SetPosition(position);

	if (input_->TriggerKey(DIK_SPACE)){
		audio_->StopWave(voiceHandle_);
	}

	//デバックテキストの表示
	ImGui::Begin("Debug1");
	ImGui::Text("Kamata Tarou %d.%d.%d", 2050, 12, 31);
	//float3入力ボックス
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	// float3入力スライダー
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);

	ImGui::ShowDemoWindow();
	ImGui::End();

	//デバックカメラの更新
	debugCamera_->Update();
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
	/*model_->Draw(worldTransform_, viewProjection_, modelTextureHandle_);*/
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), modelTextureHandle_);
	PrimitiveDrawer::GetInstance()->DrawLine3d(
		{0.0f, 0.0f, 0.0f}, {-100.0f, 100.0f, 1000.0f}, {1.0f, 0.0f, 0.0f, 1.0f});

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion
	
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite_->Draw();
	
	// スプライト描画後処理
	Sprite::PostDraw();
	
#pragma endregion
}
