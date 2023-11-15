#pragma once
#include "Data/ResourceHandles.h"
#include "Data/MaterialInfo.h"

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