#pragma once

#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"

class Enemy {
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

private: // 変数
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};
