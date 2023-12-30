#pragma once
#include "Base3DCamera.h"

class GameOverCamera : public Base3DCamera
{
public:
	GameOverCamera();
	~GameOverCamera();

	void Initialize() override;

	void Update() override;

private:

};

