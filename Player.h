#pragma once

#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"

class Player {

public:
	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection& viewProjection);

private:
	Input* input_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};