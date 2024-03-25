#pragma once
#include "Base/WinApp/WinApp.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Manager/ImGuiManager.h"
#include "Manager/TextureManager.h"
#include "Manager/InputManager.h"
#include "Manager/AudioManager.h"
#include "Manager/ParticleManager.h"
#include "Manager/RandomManager.h"
#include "Object/Sprite.h"
#include "Object/Model.h"
#include "GameObject/Camera/MainCamera.h"
#include "GameObject/Camera/InGameCamera.h"
#include "GameObject/Camera/DebugCamera.h"
#include "GameObject/Camera/SpriteCamera.h"
#include "GameObject/Light/LightObject.h"
#include "GameObject/Shadow/Shadow.h"
#include "GameObject/Particle/testParticle.h"
#include "GameObject/Particle/Object/PlaneParticle.h"
#include "GameObject/Particle/Sprite/SpriteParticle.h"
#include "Scene/IScene.h"

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
	//シャドウ
	std::unique_ptr<Shadow> shadow_;

	//ブレンドモード
	int blendMode_;

	//テクスチャハンドル
	uint32_t monsterBallHandle_;
	uint32_t fenceHandle_;

	//描画モデル
	std::unique_ptr<TestParticle> testParticle1_;

	std::shared_ptr<Model> yukariModel_;
	RenderItem yukariModelInfo_;

	std::shared_ptr<Model> groundModel_;
	RenderItem groundModelInfo_;

	std::shared_ptr<Model> cubeModel_;
	RenderItem cubeModelInfo_;

	std::shared_ptr<Model> testModel_;
	RenderItem testModelInfo_;

	std::shared_ptr<Sprite> sprite_;
	SpriteItem spriteInfo_;

	//サウンド
	uint32_t soundHandle_;
};