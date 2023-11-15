#pragma once
#include "Base/WinApp/WinApp.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Manager/ImGuiManager.h"
#include "Manager/InputManager.h"
#include "Manager/ParticleManager.h"
#include "Object/Triangle.h"
#include "Object/Sprite.h"
#include "Object/Plane.h"
#include "Object/Sphere.h"
#include "Object/Model.h"
#include "GameObject/Camera/GameCamera.h"
#include "GameObject/Camera/DebugCamera.h"
#include "GameObject/Camera/SpriteCamera.h"
#include "GameObject/Light/DirectionalLight.h"
#include "GameObject/Particle/testParticle.h"
#include "Scene/IScene.h"

class InGameScene : public IScene {
public:
	InGameScene();
	~InGameScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:
	//基本機能
	WinApp* winApp_;
	DirectXCommon* directXCommon_;
	InputManager* input_;
	//ゲームカメラ
	Matrix4x4 viewProjectionMatrix_;
	//ゲームカメラ
	std::unique_ptr<GameCamera> gameCamera_;
	//デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	bool isDebugCamera_;
	//スプライト用カメラ
	std::unique_ptr<SpriteCamera> spriteCamera_;
	//平行ライト
	std::unique_ptr<DirectionalLight> directionalLight_;
	//ブレンドモード
	int blendMode_;

	//テクスチャハンドル
	uint32_t monsterBallHandle_;
	uint32_t fenceHandle_;

	//描画モデル
	std::unique_ptr<TestParticle> testParticle_;

	std::unique_ptr<Model> groundModel_;
	RenderItem groundModelInfo_;

	std::unique_ptr<Sphere> monsterBall_;
	RenderItem monsterBallInfo_;

	std::unique_ptr<Sprite> sprite_;
	RenderItem spriteInfo_;
};