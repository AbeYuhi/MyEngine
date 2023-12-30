#pragma once
#include "Base3DCamera.h"

class MenuCamera : public Base3DCamera
{
public:
	MenuCamera();
	~MenuCamera();

	void Initialize() override;

	void Update() override;

private:
};

