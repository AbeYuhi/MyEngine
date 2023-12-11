#pragma once
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "DirectXGame/Manager/InputManager.h"
#include "DirectXGame/Manager/AudioManager.h"
#include "DirectXGame/Manager/ParticleManager.h"
#include "DirectXGame/Manager/RandomManager.h"
#include "DirectXGame/Object/Triangle.h"
#include "DirectXGame/Object/Sprite.h"
#include "DirectXGame/Object/Plane.h"
#include "DirectXGame/Object/Sphere.h"
#include "DirectXGame/Object/Model.h"
#include "DirectXGame/Object/WireFrameBox.h"
#include "DirectXGame/GameObject/Camera/MainCamera.h"
#include "DirectXGame/GameObject/Camera/InGameCamera.h"
#include "DirectXGame/GameObject/Camera/DebugCamera.h"
#include "DirectXGame/GameObject/Camera/SpriteCamera.h"
#include "DirectXGame/GameObject/Light/LightObject.h"
#include "DirectXGame/GameObject/Particle/testParticle.h"
#include "DirectXGame/Scene/IScene.h"

class InGameScene : public IScene {
public:
	InGameScene();
	~InGameScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:
	//基本機能
	WinApp* winApp_;
	DirectXCommon* directXCommon_;
	InputManager* input_;
	AudioManager* audioManager_;
	RandomManager* randomManager_;
	MainCamera* mainCamera_;
	SpriteCamera* spriteCamera_;
	//ゲームカメラ
	std::unique_ptr<InGameCamera> gameCamera_;
	//デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	bool isDebugCamera_;
	//ライト
	std::unique_ptr<LightObject> lightObj_;

	//ブレンドモード
	int blendMode_;

	//テクスチャハンドル
	uint32_t monsterBallHandle_;
	uint32_t fenceHandle_;

	Vector2 windowPos_;

	//描画モデル
	std::unique_ptr<TestParticle> testParticle1_;

	std::unique_ptr<Model> groundModel_;
	RenderItem groundModelInfo_;

	std::unique_ptr<Sphere> monsterBall_;
	RenderItem monsterBallInfo_;

	std::unique_ptr<Sprite> sprite_;
	RenderItem spriteInfo_;

	std::unique_ptr<WireFrameBox> wireFrameBox_;
	RenderItem wireFrameBoxInfo_;

	//サウンド
	uint32_t soundHandle_;
};