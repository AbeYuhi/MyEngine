#pragma once
#include "Scene/IScene.h"

class TitleScene : public IScene
{
public:
	TitleScene();
	~TitleScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:

};