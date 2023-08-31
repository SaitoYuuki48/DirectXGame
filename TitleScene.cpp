#include "TitleScene.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() { 
	delete sprite2DTitle_;
	delete spriteEnter_;
}

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// テクスチャ
	uint32_t textureTitle = TextureManager::Load("title.png");

	// スプライト生成
	sprite2DTitle_ =
	    Sprite::Create(textureTitle, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});

	uint32_t textureEnter = TextureManager::Load("enter.png");

	spriteEnter_ =
	    Sprite::Create(textureEnter, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});

	isSceneEnd = false;
}

void TitleScene::Update() {
	if (input_->TriggerKey(DIK_RETURN)) {
		isSceneEnd = true;
	}
}

void TitleScene::Draw() { 
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite2DTitle_->Draw();

	spriteTimer_--;

	if (spriteTimer_ == 0) {
		spriteTimer_ = 30;
	}

	if (spriteTimer_ <= 30 && spriteTimer_ >= 15)	{
		spriteEnter_->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
