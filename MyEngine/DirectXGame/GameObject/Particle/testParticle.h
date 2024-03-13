#pragma once
#include <memory>
#include "DirectXGame/Manager/ParticleManager.h"
#include "DirectXGame/GameObject/Particle/Object/PlaneParticle.h"
#include "DirectXGame/Object/Model.h"
#include "DirectXGame/Object/Sprite.h"

class TestParticle
{
public:

	void Initialize();

	void Update();

	void Draw();

private:
	std::unique_ptr<PlaneParticle> particle0_;
	std::unique_ptr<PlaneParticle> particle1_;

	Vector3 pos;
};