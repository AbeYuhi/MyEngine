#pragma once
#include "IScene.h"

class GameOverScene : public IScene
{
public:
	GameOverScene();
	~GameOverScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;
private:

};