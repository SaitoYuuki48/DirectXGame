#include "RailCamera.h"
#include "ImGuiManager.h"

void RailCamera::Initialize() {
	// x,y,z方向の回転を設定
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	// x,y,zの方向のを設定
	worldTransform_.translation_ = {0.0f, 0.0f, -50.0f};

	// ビュープロジェクションの初期化
	//viewProjection_.farZ = 600;
	viewProjection_.Initialize();
}

void RailCamera::Update() {
	// カメラの速さ
	//const float kCameraSpeed = 0.02f;

	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	//worldTransform_.translation_.x += velocity_.x;
	//worldTransform_.translation_.y += velocity_.y;
	//worldTransform_.translation_.z += kCameraSpeed;

	//worldTransform_.rotation_.z += kCameraSpeed;

	worldTransform_.matWorld_ = MakeAffineMatrix({1.0f, 1.0f, 1.0f}, worldTransform_.rotation_, worldTransform_.translation_);

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	
	//カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	float cameraPos[] = {
	    worldTransform_.translation_.x, 
		worldTransform_.translation_.y,
	    worldTransform_.translation_.z
	};
	ImGui::SliderFloat3("cameraPos", cameraPos, 0.0f, 128.0f);
	float cameraRot[] = {
	    worldTransform_.rotation_.x,
		worldTransform_.rotation_.y,
		worldTransform_.rotation_.z
	};
	ImGui::SliderFloat3("cameraRot", cameraRot, -128.0f, 128.0f);
	ImGui::End();

	//ビュープロジェクションの位置
	/*ImGui::Begin("viewProjection");
	float viewProjectionPos[] = {
	    viewProjection_.translation_.x, 
		viewProjection_.translation_.y,
	    viewProjection_.translation_.z};
	ImGui::SliderFloat3("viewProjection", viewProjectionPos, 0.0f, 128.0f);
	ImGui::End();*/
}
