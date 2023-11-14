#pragma once

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

	inline int GetSceneNo() { return sceneNo_; }

protected:
	static int debugMode_;
	static int sceneNo_;
};