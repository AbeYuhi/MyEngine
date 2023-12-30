#pragma once
#include "IScene.h"
#include "DirectXGame/GameObject/Camera/TitleCamera.h"
#include "DirectXGame/GameObject/Camera/MainCamera.h"
#include "DirectXGame/GameObject/Camera/SpriteCamera.h"

class TitleScene : public IScene
{
public:
	TitleScene();
	~TitleScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:
	MainCamera* mainCamera_;
	SpriteCamera* spriteCamera_;

	std::unique_ptr<TitleCamera> titleCamera_;

};