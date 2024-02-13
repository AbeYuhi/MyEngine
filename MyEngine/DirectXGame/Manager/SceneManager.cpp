#include "SceneManager.h"

SceneManager* SceneManager::GetInstance() {
	static SceneManager instance;
	return &instance;
}

void SceneManager::Initialize(GameScene gameScene){

	//シーンファクトリー
	sceneFactory_ = SceneFactory::GetInstance();

	//初期シーン
	sceneNo_ = gameScene;
	//初期ゲームシーンの初期化
	scene_ = sceneFactory_->CreateScene((GameScene)sceneNo_);

	//初期シーンの初期化
	scene_->Initialize();
}

void SceneManager::Update(){
	preSceneNo_ = sceneNo_;
	sceneNo_ = scene_->GetSceneNo();

#ifdef _DEBUG
	ImGui::Begin("GameScene");
	const char* modes[] = { "Title", "InGame", "Menu", "GameOver", "GameClear" };
	ImGui::Combo("gameScene", &sceneNo_, modes, IM_ARRAYSIZE(modes));
	ImGui::End();
#endif // _DEBUG

	if (preSceneNo_ != sceneNo_) {
		//シーンの削除
		scene_->Finalize();
		scene_.reset();
		//シーンの生成
		scene_ = sceneFactory_->CreateScene((GameScene)sceneNo_);
		//シーンの初期化
		scene_->Initialize();
	}

	//シーンの更新
	scene_->Update();
}

void SceneManager::Draw(){
	//シーンの描画
	scene_->Draw();
}
