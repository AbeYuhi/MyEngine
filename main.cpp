#include "WinApp/WinApp.h"
#include "DirectXCommon/DirectXCommon.h"
#include "Debug/DebugLog.h"

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

	//メインループ
	while (true) {
		if (winApp->ProcessMessage()) {
			break;
		}

		///更新処理


		///描画処理
		//画面初期化
		directXCommon->PreDraw();
		
		//描画終了
		directXCommon->PostDraw();
	}

	return 0;
}