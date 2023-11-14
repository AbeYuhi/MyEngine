#include "testParticle.h"


TestParticle::TestParticle(){}
TestParticle::TestParticle(const Matrix4x4* viewProjectionMatrix, int maxParticleCount) : ParticleManager(viewProjectionMatrix, maxParticleCount){}

void TestParticle::Initialize() {
	//基本機能の初期化
	ParticleManager::Initialize();

	//パーティクルの初期化
	plane_ = Plane::Create();
	plane_->Initialize();
}

void TestParticle::Update() {

	//パーティクルの更新
	ParticleManager::Update();
}

void TestParticle::Draw() {
	plane_->Draw(worldTransformResource_.Get(), materialInfo_, kMaxParticleCount_, srvHadnelGPU_);
}