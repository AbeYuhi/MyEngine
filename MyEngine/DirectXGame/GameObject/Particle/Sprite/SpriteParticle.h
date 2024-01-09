#pragma once
#include "DirectXGame/Manager/ParticleManager.h"
#include "DirectXGame/Manager/ObjectManager.h"
#include "DirectXGame/Object/Plane.h"
#include "DirectXGame/Object/Model.h"

class SpriteParticle : public ParticleManager
{
public:

	SpriteParticle(int maxParticleCount);
	~SpriteParticle() = default;

	void Initialize() override;

	void Update() override;

	void EmitterDraw() override;

	void Draw() override;

private: //メンバ関数

	ParticleInfo MakeNewParticle() override;

private:
	Sprite* particleSprite_;
	SpriteData spriteData_;
	uint32_t textureHandle_;
};