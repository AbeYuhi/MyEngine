#pragma once
#include "Base3DCamera.h"

class GameClearCamera : public Base3DCamera
{
public:
	GameClearCamera();
	~GameClearCamera();

	void Initialize() override;

	void Update() override;

private:
};

