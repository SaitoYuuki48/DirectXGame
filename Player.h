#pragma once
#include "Input.h"

class Player {

public:
	void Initialize();

	void Update();

	void Draw();

private:
	Input* input_ = nullptr;
	
};
