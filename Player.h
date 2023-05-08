#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"

class Player {

public:
	void Initialize();

	void Update();

	void Draw();

private:
	Input* input_ = nullptr;

	WorldTransform worldTransform_;
};
