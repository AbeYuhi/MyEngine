#pragma once
#include "DirectXGame/Manager/ParticleManager.h"
#include "DirectXGame/Object/Model.h"
#include "DirectXGame/Object/Sprite.h"

class TestParticle : public ParticleManager
{
public:

	TestParticle(int maxParticleCount);
	~TestParticle() = default;

	void Initialize() override;

	void Update() override;

	void EmitterDraw() override;

	void Draw() override;

private: //メンバ関数

	ParticleInfo MakeNewParticle() override;

private:
	std::shared_ptr<Model> particleModel_;
	std::shared_ptr<Sprite> particleSprite_;
	SpriteData spriteData_;
	uint32_t textureHandle_;

	//風フィールド
	AccelerationField accelerationField_;

	//風が吹いているか
	bool isAccelerationField_;
};