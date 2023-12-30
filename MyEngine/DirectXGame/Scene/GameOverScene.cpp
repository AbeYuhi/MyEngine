#include "GameOverScene.h"

GameOverScene::GameOverScene() {}

GameOverScene::~GameOverScene() {}

void GameOverScene::Finalize() {

}

void GameOverScene::Initialize() {
	sceneNo_ = GAMEOVER;

	mainCamera_ = MainCamera::GetInstance();
	spriteCamera_ = SpriteCamera::GetInstance();
}

void GameOverScene::Update() {

}

void GameOverScene::Draw() {

}