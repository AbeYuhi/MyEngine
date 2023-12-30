#pragma once
#include "IScene.h"
#include "DirectXGame/GameObject/Camera/GameClearCamera.h"
#include "DirectXGame/GameObject/Camera/MainCamera.h"
#include "DirectXGame/GameObject/Camera/SpriteCamera.h"

class GameClearScene : public IScene
{
public:
	GameClearScene();
	~GameClearScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;
private:
	MainCamera* mainCamera_;
	SpriteCamera* spriteCamera_;

};