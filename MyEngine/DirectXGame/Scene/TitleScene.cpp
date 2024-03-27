#include "TitleScene.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {}

void TitleScene::Finalize() {

}

void TitleScene::Initialize() {
	sceneNo_ = TITLE;

	//
	mainCamera_ = MainCamera::GetInstance();
	spriteCamera_ = SpriteCamera::GetInstance();

	//カメラ
	titleCamera_ = std::make_unique<TitleCamera>();
	titleCamera_->Initialize();
}

void TitleScene::Update() {
	titleCamera_->Update();


}

void TitleScene::Draw() {


}