#pragma once
#include "DirectXGame/Manager/TextureManager.h"

enum GameScene {
	TITLE, INGAME, MENU, GAMEOVER, GAMECLEAR, SCENENUM
};

class IScene
{
public:
	IScene() = default;
	virtual ~IScene() = default;

	virtual void Initialize() = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual void Finalize() = 0;

	inline int GetSceneNo() { return sceneNo_; }

	//画像ハンドル
	static uint32_t uvCheckerHandle_;
	static uint32_t whiteTextureHandle_;

protected:
	static bool debugMode_;
	static int sceneNo_;
};