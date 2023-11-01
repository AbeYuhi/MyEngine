#include "DebugSytem/DebugLog.h"
#include "DebugSytem/ReleseCheck.h"
#include "Base/SafeDelete/SafeDelete.h"
#include "Base/WinApp/WinApp.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Manager/ImGuiManager.h"
#include "Manager/TextureManager.h"
#include "Manager/GraphicsPipelineManager.h"
#include "Object/Triangle.h"
#include "Object/Sprite.h"
#include "Object/Sphere.h"
#include "Scene/GameScene.h"

//エントリーポイント
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int) {
	//リソースリークチェック
	static D3DResoruceLeakCheaker resourceLeakCheker{};
	//汎用機能
	WinApp* winApp = nullptr;
	DirectXCommon* directXCommon = nullptr;
	ImGuiManager* imGuiManager = nullptr;
	TextureManager* textureManager = nullptr;
	GraphicsPipelineManager* graphicsPipelineManager = nullptr;
	GameScene* gameScene = nullptr;
	
	//ゲームウィンドウの作成
	winApp = WinApp::GetInstance();
	winApp->CreateGameWindow();

	//DirectXCommonの初期化
	directXCommon = DirectXCommon::GetInstance();
	directXCommon->Initialize();

#pragma region 汎用機能
	//ImGuiマネージャーの初期化
	imGuiManager = ImGuiManager::GetInstance();
	imGuiManager->Initialize();

	//テクスチャマネージャーの初期化
	textureManager = TextureManager::GetInstance();
	textureManager->Initialize();

	graphicsPipelineManager = GraphicsPipelineManager::GetInstance();
	graphicsPipelineManager->Initialize();

	//ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize();

#pragma endregion

	//メインループ
	while (true) {
		if (winApp->ProcessMessage()) {
			break;
		}

		///更新処理
		//ImGuiの受付開始
		imGuiManager->Begin();

		//ゲームシーンの更新
		gameScene->Update();

		//ImGuiの受付終了
		imGuiManager->End();

		///描画処理
		//画面初期化
		directXCommon->PreDraw();
		
		//ゲームシーンの描画
		gameScene->Draw();

		//ImGuiの描画
		imGuiManager->Draw();

		//描画終了
		directXCommon->PostDraw();
	}

	//ゲームシーンのDelete
	SafeDelete(gameScene);
	//ImGuiの解放処理
	imGuiManager->ReleseProcess();
	//ゲームウィンドウの破棄
	winApp->DiscardingWindow();
	return 0;
}