#include "Manager/GameManager.h"

GameManager* GameManager::GetInstance() {
	static GameManager instance;
	return &instance;
}

void GameManager::Initialize() {
	//タイトルネーム
	const wchar_t title[] = L"MyEngine";
	//基本機能の初期化
	//ゲームウィンドウの作成
	winApp_ = WinApp::GetInstance();
	winApp_->CreateGameWindow(title);

	//DirectXCommonの初期化
	directXCommon_ = DirectXCommon::GetInstance();
	directXCommon_->Initialize();

#pragma region 汎用機能
	//入力処理マネージャーの初期化
	inputManager_ = InputManager::GetInstance();
	inputManager_->Initialize();

	//ImGuiマネージャーの初期化
	imGuiManager_ = ImGuiManager::GetInstance();
	imGuiManager_->Initialize();

	//汎用テクスチャの読み込み
	IScene::uvCheckerHandle_ = TextureManager::Load("uvChecker.png");
	IScene::whiteTextureHandle_ = TextureManager::Load("whiteTexture2x2.png");

	graphicsPipelineManager_ = GraphicsPipelineManager::GetInstance();
	graphicsPipelineManager_->Initialize();

#pragma endregion

	//ゲームシーンの生成
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[INGAME] = std::make_unique<InGameScene>();
	sceneArr_[MENU] = std::make_unique<MenuScene>();
	sceneArr_[GAMEOVER] = std::make_unique<GameOverScene>();
	sceneArr_[GAMECLEAR] = std::make_unique<GameClearScene>();
	//初期ゲームシーンの設定
	sceneNo_ = INGAME;
	//初期ゲームシーンの初期化
	sceneArr_[sceneNo_]->Initialize();
}

int GameManager::Run() {

	//初期化
	Initialize();

	//メインループ
	while (true) {
		if (winApp_->ProcessMessage()) {
			break;
		}

		///更新処理
		//入力受付
		inputManager_->Update();
		//ImGuiの受付開始
		imGuiManager_->Begin();
		//ゲームシーンの更新
		preSceneNo_ = sceneNo_;
		sceneNo_ = sceneArr_[sceneNo_]->GetSceneNo();
		ImGui::Begin("GameScene");
		const char* modes[] = { "Title", "InGame", "Menu", "GameOver", "GameClear" };
		ImGui::Combo("gameScene", &sceneNo_, modes, IM_ARRAYSIZE(modes));
		ImGui::End();
		if (preSceneNo_ != sceneNo_) {
			sceneArr_[sceneNo_]->Initialize();
		}
		sceneArr_[sceneNo_]->Update();
		//ImGuiの受付終了
		imGuiManager_->End();

		///描画処理
		//画面初期化
		directXCommon_->PreDraw();
		//ゲームシーンの描画
		sceneArr_[sceneNo_]->Draw();
		//ImGuiの描画
		imGuiManager_->Draw();
		//描画終了
		directXCommon_->PostDraw();
	}

	//ImGuiの解放処理
	imGuiManager_->ReleseProcess();
	//ゲームウィンドウの破棄
	winApp_->DiscardingWindow();
	return 0;
}