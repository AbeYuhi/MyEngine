#include "SceneFactory.h"

SceneFactory* SceneFactory::GetInstance() {
	static SceneFactory instance;
	return &instance;
}

std::unique_ptr<IScene> SceneFactory::CreateScene(GameScene gameScene) {

	std::unique_ptr<IScene> scene = nullptr;

	switch (gameScene)
	{
	case TITLE:
		scene = std::make_unique<TitleScene>();
		break;
	case INGAME:
	default:
		scene = std::make_unique<InGameScene>();
		break;
	case MENU:
		scene = std::make_unique<MenuScene>();
		break;
	case GAMEOVER:
		scene = std::make_unique<GameOverScene>();
		break;
	case GAMECLEAR:
		scene = std::make_unique<GameClearScene>();
		break;
	}

	return scene;
}
