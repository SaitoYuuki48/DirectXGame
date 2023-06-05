#pragma once

#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"

class PlayerBullet {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& position);

	///<summary>
	///更新
	/// </summary>
	void Update();

	///< summary>
	/// 描画
	///  </summary>
	/// <param name="viewPrejection& viewProjection
	void Draw(const ViewProjection& viewProjection);

private:
	//Input* input_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};
