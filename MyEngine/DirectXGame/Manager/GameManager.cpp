#include "GameManager.h"

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

	//AudioManagerの初期化
	audioManager_ = AudioManager::GetInstance();
	audioManager_->Initialize();

	graphicsPipelineManager_ = GraphicsPipelineManager::GetInstance();
	graphicsPipelineManager_->Initialize();

#pragma endregion

	//FPS計測の初期化
	deltaTime_ = 0.0;
	fps_ = 0.0;
	frameCount_ = 0;
	totalTime_ = 0.0;

	//初期ゲームシーンの設定
	sceneNo_ = INGAME;
	//初期ゲームシーンの初期化
	switch (sceneNo_)
	{
	case TITLE:
		sceneArr_[sceneNo_] = std::make_unique<TitleScene>();
		break;
	case INGAME:
	default:
		sceneArr_[sceneNo_] = std::make_unique<InGameScene>();
		break;
	case MENU:
		sceneArr_[sceneNo_] = std::make_unique<MenuScene>();
		break;
	case GAMEOVER:
		sceneArr_[sceneNo_] = std::make_unique<GameOverScene>();
		break;
	case GAMECLEAR:
		sceneArr_[sceneNo_] = std::make_unique<GameClearScene>();
		break;
	}
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

		//フレームの開始時刻を取得
		auto frameStart = std::chrono::high_resolution_clock::now();
		///更新処理
		//入力受付
		inputManager_->Update();
		//ImGuiの受付開始
		imGuiManager_->Begin();
		//現在のFPS値
		ImGui::Begin("FPS");
		ImGui::Text("fps : %lf", fps_);
		ImGui::End();
		//ゲームシーンの更新
		preSceneNo_ = sceneNo_;
		sceneNo_ = sceneArr_[sceneNo_]->GetSceneNo();
		ImGui::Begin("GameScene");
		const char* modes[] = { "Title", "InGame", "Menu", "GameOver", "GameClear" };
		ImGui::Combo("gameScene", &sceneNo_, modes, IM_ARRAYSIZE(modes));
		ImGui::End();
		if (preSceneNo_ != sceneNo_) {
			sceneArr_[preSceneNo_]->Finalize();
			sceneArr_[preSceneNo_].reset();
			switch (sceneNo_)
			{
			case TITLE:
				sceneArr_[sceneNo_] = std::make_unique<TitleScene>();
				break;
			case INGAME:
			default:
				sceneArr_[sceneNo_] = std::make_unique<InGameScene>();
				break;
			case MENU:
				sceneArr_[sceneNo_] = std::make_unique<MenuScene>();
				break;
			case GAMEOVER:
				sceneArr_[sceneNo_] = std::make_unique<GameOverScene>();
				break;
			case GAMECLEAR:
				sceneArr_[sceneNo_] = std::make_unique<GameClearScene>();
				break;
			}
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

		// フレーム終了時刻を取得
		auto frameEnd = std::chrono::high_resolution_clock::now();

		// フレームごとの時間を計算
		std::chrono::duration<double> frameDuration = frameEnd - frameStart;
		deltaTime_ = frameDuration.count();

		//FPSの更新
		totalTime_ += deltaTime_;
		frameCount_++;
		if (totalTime_ > 1.0f) {
			fps_ = frameCount_ / totalTime_;
			frameCount_ = 0;
			totalTime_ = 0;
		}
	}

	//音声データの解放
	audioManager_->Finalize();
	//ImGuiの解放処理
	imGuiManager_->ReleseProcess();
	//ゲームウィンドウの破棄
	winApp_->DiscardingWindow();
	return 0;
}