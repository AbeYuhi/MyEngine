#include "GameManager.h"

GameManager* GameManager::GetInstance() {
	static GameManager instance;
	return &instance;
}

void GameManager::Initialize() {
	
	//汎用機能の初期化
	Framework::Initialize();

	//汎用テクスチャの読み込み
	IScene::uvCheckerHandle_ = TextureManager::Load("uvChecker.png");
	IScene::whiteTextureHandle_ = TextureManager::Load("whiteTexture2x2.png");

	//メインカメラの初期化
	mainCamera_ = MainCamera::GetInstance();
	mainCamera_->Initialize();

#pragma endregion

	//FPS計測の初期化
	deltaTime_ = 0.0;
	fps_ = 0.0;
	frameCount_ = 0;
	totalTime_ = 0.0;

	//シーンマネージャーの初期化(初期シーンの設定)
	sceneManager_ = SceneManager::GetInstance();
	sceneManager_->Initialize(INGAME);

}

void GameManager::Finalize() {

	//ゲームの終了処理


	//フレームワークの終了処理
	Framework::Finalize();
}

int GameManager::Run() {

	//フレームワークの初期化
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
#ifdef _DEBUG
		//ImGuiの受付開始
		imGuiManager_->Begin();
		//グローバル変数の更新
		globalVariables_->Updates();
		//現在のFPS値
		ImGui::Begin("FPS");
		ImGui::Text("fps : %lf", fps_);
		ImGui::End();
#endif // _DEBUG
		//シーンの更新
		sceneManager_->Update();

#ifdef _DEBUG
		//ImGuiの受付終了
		imGuiManager_->End();
#endif // _DEBUG

		///描画処理
		//画面初期化
		directXCommon_->PreDraw();
		//シーンの描画
		sceneManager_->Draw();

#ifdef _DEBUG
		//ImGuiの描画
		imGuiManager_->Draw();
#endif // _DEBUG

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

	//ゲーム終了処理
	Finalize();

	return 0;
}