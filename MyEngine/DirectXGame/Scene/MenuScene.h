#pragma once
#include "IScene.h"
#include "DirectXGame/GameObject/Camera/MenuCamera.h"
#include "DirectXGame/GameObject/Camera/MainCamera.h"
#include "DirectXGame/GameObject/Camera/SpriteCamera.h"

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
	MainCamera* mainCamera_;
	SpriteCamera* spriteCamera_;

};
