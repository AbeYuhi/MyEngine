#include "Framework.h"

void Framework::Initialize() {
	//タイトルネーム
	const wchar_t title[] = L"MyEngine";

#pragma region エンジン部分

	//ゲームウィンドウの作成
	winApp_ = WinApp::GetInstance();
	winApp_->CreateGameWindow(title);

	//DirectXCommonの初期化
	directXCommon_ = DirectXCommon::GetInstance();
	directXCommon_->Initialize();

#pragma endregion

#pragma region 汎用機能
	//入力処理マネージャーの初期化
	inputManager_ = InputManager::GetInstance();
	inputManager_->Initialize();

#ifdef _DEBUG
	//ImGuiマネージャーの初期化
	imGuiManager_ = ImGuiManager::GetInstance();
	imGuiManager_->Initialize();
#endif //_DEBUG

	//JSONの読み書き
	globalVariables_ = GlobalVariables::GetInstance();
	globalVariables_->LoadFiles();

	//AudioManagerの初期化
	audioManager_ = AudioManager::GetInstance();
	audioManager_->Initialize();

	//RandomManagerの初期化
	randomManager_ = RandomManager::GetInstance();
	randomManager_->Initialize();

	collisionManager_ = CollisionManager::GetInstance();
	collisionManager_->Initialize();

	graphicsPipelineManager_ = GraphicsPipelineManager::GetInstance();
	graphicsPipelineManager_->Initialize();

#pragma endregion

}

void Framework::Finalize() {
	//音声データの解放
	audioManager_->Finalize();

#ifdef _DEBUG
	//ImGuiの解放処理
	imGuiManager_->ReleseProcess();
#endif // _DEBUG

	//ゲームウィンドウの破棄
	winApp_->DiscardingWindow();
}