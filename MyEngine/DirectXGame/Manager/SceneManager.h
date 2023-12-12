#pragma once
#include <memory>
#include "DirectXGame/Scene/IScene.h"
#include "DirectXGame/Scene/SceneFactory/SceneFactory.h"
#include "DirectXGame/Manager/ImGuiManager.h"

class SceneManager
{
public:
	static SceneManager* GetInstance();

	/// <summary>
	/// シーンマネージャーの初期化
	/// </summary>
	/// <param name="gameScene">初期シーン</param>
	void Initialize(GameScene gameScene = GameScene::TITLE);

	/// <summary>
	/// シーンの更新
	/// </summary>
	void Update();

	/// <summary>
	/// シーンの描画
	/// </summary>
	void Draw();

private:
	SceneManager() = default;
	~SceneManager() = default;

	//シーンファクトリー
	SceneFactory* sceneFactory_;

	//ゲームシーン
	std::unique_ptr<IScene> scene_;
	int sceneNo_;
	int preSceneNo_;
};

