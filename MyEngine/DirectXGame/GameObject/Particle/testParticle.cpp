#include "testParticle.h"


TestParticle::TestParticle(const Matrix4x4* viewProjectionMatrix, int maxParticleCount) : ParticleManager(viewProjectionMatrix, maxParticleCount){}

void TestParticle::Initialize() {
	//基本機能の初期化
	ParticleManager::Initialize();

	//パーティクルの初期化
	plane_ = Plane::Create();
	//model_ = Model::Create("bunny");

	textureHandle_ = TextureManager::Load("uvChecker.png");

	int index = 0;
	for (std::list<ParticleInfo>::iterator itParticle = particles_.begin(); itParticle != particles_.end(); itParticle++) {
		ParticleInfo* particle = &(*itParticle);
		index++;
	}
}

void TestParticle::Update() {

	for (std::list<ParticleInfo>::iterator itParticle = particles_.begin(); itParticle != particles_.end(); itParticle++) {
		ParticleInfo* particle = &(*itParticle);

		particle->srtData_.rotate_.y += 0.05f;
	}

	//パーティクルの更新
	ParticleManager::Update();
}

void TestParticle::Draw() {
	plane_->Draw(drawInfo_, textureHandle_);
	model_->Draw(drawInfo_);
}