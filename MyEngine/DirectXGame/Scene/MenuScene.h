#pragma once
#include "IScene.h"

class MenuScene : public IScene
{
public:
	MenuScene();
	~MenuScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:

};
