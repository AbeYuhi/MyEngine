#pragma once
#include "Manager/ParticleManager.h"
#include "Object/Plane.h"
#include "Object/Model.h"

class TestParticle : public ParticleManager
{
public:

	TestParticle(const Matrix4x4* viewProjectionMatrix, int maxParticleCount);
	~TestParticle() = default;

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:
	std::unique_ptr<Plane> plane_;
	uint32_t textureHandle_;

	std::unique_ptr<Model> model_;
};