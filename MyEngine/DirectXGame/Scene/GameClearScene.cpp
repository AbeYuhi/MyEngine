#include "GameClearScene.h"

GameClearScene::GameClearScene() {}
GameClearScene::~GameClearScene() {}

void GameClearScene::Finalize() {
	
}

void GameClearScene::Initialize() {
	GAMECLEAR;

	mainCamera_ = MainCamera::GetInstance();
	spriteCamera_ = SpriteCamera::GetInstance();
}

void GameClearScene::Update() {

}

void GameClearScene::Draw() {

}