#pragma once
#include "Scene/IScene.h"

class MenuScene : public IScene
{
public:
	MenuScene();
	~MenuScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:

};
