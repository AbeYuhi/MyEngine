#include "Debug/DebugLog.h"
#include "Base/SafeDelete/SafeDelete.h"
#include "Base/WinApp/WinApp.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Object/Triangle.h"
#include "Scene/GameScene.h"


//エントリーポイント
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int) {
	//汎用機能
	WinApp* winApp = nullptr;
	DirectXCommon* directXCommon = nullptr;

	//ゲームウィンドウの作成
	winApp = WinApp::GetInstance();
	winApp->CreateGameWindow();

	//DirectXCommonの初期化
	directXCommon = DirectXCommon::GetInstance();
	directXCommon->Initialize();

	//オブジェクトの初期化
	Triangle::StaticInitialize();

	//ゲームシーンの初期化
	GameScene* gameScene = new GameScene();
	gameScene->Initialize();

	//メインループ
	while (true) {
		if (winApp->ProcessMessage()) {
			break;
		}

		///更新処理

		//ゲームシーンの更新
		gameScene->Update();

		///描画処理
		//画面初期化
		directXCommon->PreDraw();
		
		//ゲームシーンの描画
		gameScene->Draw();

		//描画終了
		directXCommon->PostDraw();
	}
	
	//ゲームシーンのDelete
	SafeDelete(gameScene);
	//ゲームウィンドウの破棄
	winApp->DiscardingWindow();

	return 0;
}