#pragma once
#include "DirectXGame/Manager/ParticleManager.h"
#include "DirectXGame/Object/Sprite.h"

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
	std::shared_ptr<Sprite> particleSprite_;
	SpriteData spriteData_;
};