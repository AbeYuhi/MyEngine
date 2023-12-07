#pragma once
#include "Base3DCamera.h"

class TitleCamera : public Base3DCamera
{
public:
	TitleCamera();
	~TitleCamera();

	void Initialize() override;

	void Update() override;

private:
};

