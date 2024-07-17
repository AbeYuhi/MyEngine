#pragma once
#include "Base/WinApp/WinApp.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Manager/ImGuiManager.h"
#include "Manager/TextureManager.h"
#include "Manager/InputManager.h"
#include "Manager/AudioManager.h"
#include "Manager/ParticleManager.h"
#include "Manager/PostEffectManager.h"
#include "Manager/RandomManager.h"
#include "Manager/LightObjectManager.h"
#include "Object/Sprite.h"
#include "Object/Model.h"
#include "Object/SkyBox.h"
#include "GameObject/Camera/MainCamera.h"
#include "GameObject/Camera/InGameCamera.h"
#include "GameObject/Camera/DebugCamera.h"
#include "GameObject/Camera/SpriteCamera.h"
#include "GameObject/Shadow/Shadow.h"
#include "GameObject/Particle/testParticle.h"
#include "GameObject/Particle/Object/PlaneParticle.h"
#include "GameObject/Particle/Sprite/SpriteParticle.h"
#include "LoadFile/LevelScene.h"
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
	PostEffectManager* postEffectManager_;
	MainCamera* mainCamera_;
	SpriteCamera* spriteCamera_;
	//ライト
	LightObjectManager* lightObj_;
	//ゲームカメラ
	std::unique_ptr<InGameCamera> gameCamera_;
	//デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	bool isDebugCamera_;
	//シャドウ
	std::unique_ptr<Shadow> shadow_;

	//ブレンドモード
	int blendMode_;

	//テクスチャハンドル
	uint32_t monsterBallHandle_;
	uint32_t fenceHandle_;
	uint32_t skyboxHandle_;

	//描画モデル
	std::unique_ptr<TestParticle> testParticle1_;

	std::shared_ptr<Model> yukariModel_;
	RenderItem yukariModelInfo_;

	std::shared_ptr<Model> walkModel_;
	std::shared_ptr<Model> sneakWalkModel_;
	std::shared_ptr<Model> boxModel_;
	RenderItem walkModelInfo_;
	RenderItem boxModelInfo_;

	std::shared_ptr<SkyBox> skybox_;
	RenderItem skyboxInfo_;

	std::shared_ptr<Sprite> sprite_;
	SpriteItem spriteInfo_;

	//サウンド
	uint32_t soundHandle_;

	LevelScene levelScene_;
};