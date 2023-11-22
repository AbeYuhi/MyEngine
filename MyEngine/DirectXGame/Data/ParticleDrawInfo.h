#pragma once
#include "DirectXGame/Data/ResourceHandles.h"
#include "DirectXGame/Data/MaterialInfo.h"

struct ParticleDrawInfo {
	void Initialize(const ResourceHandles* srvHandle, const ParticleMaterialInfo* materialInfo, const int* kMaxParticleCount) {
		srvHandle_ = srvHandle;
		materialInfo_ = materialInfo;
		kMaxParticleCount_ = kMaxParticleCount;
	}
	const ResourceHandles* srvHandle_;
	const ParticleMaterialInfo* materialInfo_;
	const int* kMaxParticleCount_;
};