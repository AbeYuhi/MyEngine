#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Manager/GameManager.h"

//エントリーポイント
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int) {
	//リソースリークチェック
	static D3DResoruceLeakCheaker resourceLeakCheker{};
	
	//ゲーム処理
	GameManager* gameManager = GameManager::GetInstance();
	gameManager->Run();

	return 0;
}