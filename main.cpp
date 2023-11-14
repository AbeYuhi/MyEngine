#include "Base/WinApp/WinApp.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Manager/GameManager.h"

//エントリーポイント
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int) {
	//リソースリークチェック
	static D3DResoruceLeakCheaker resourceLeakCheker{};
	
	//ゲーム処理
	GameManager* gameManager = GameManager::GetInstance();
	gameManager->Run();

	return 0;
}