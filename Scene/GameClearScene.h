#pragma once
#include "Scene/IScene.h"

class GameClearScene : public IScene
{
public:
	GameClearScene();
	~GameClearScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;
private:

};