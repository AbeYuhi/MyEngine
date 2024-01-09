#include "MaterialInfo.h"

void MaterialInfo::Initialize() {
	CreateResource();
	RunMap();

	material_->Initialize();
	uvTransform_.Initialize();
	isInvisible_ = false;

	UpdateMatrix();
}

void MaterialInfo::CreateResource() {
	resource_ = CreateBufferResource(sizeof(Material));
}

void MaterialInfo::RunMap() {
	resource_->Map(0, nullptr, reinterpret_cast<void**>(&material_));
}

void MaterialInfo::UpdateMatrix() {
	material_->uvTransform = MakeAffineMatrix(uvTransform_.scale_, uvTransform_.rotate_, uvTransform_.translate_);
}

void ParticleMaterialInfo::Initialize() {
	CreateResource();
	RunMap();

	material_->Initialize();
	uvTransform_.Initialize();
	isInvisible_ = false;

	UpdateMatrix();
}

void ParticleMaterialInfo::CreateResource() {
	resource_ = CreateBufferResource(sizeof(ParticleMaterial));
}

void ParticleMaterialInfo::RunMap() {
	resource_->Map(0, nullptr, reinterpret_cast<void**>(&material_));
}

void ParticleMaterialInfo::UpdateMatrix() {
	material_->uvTransform = MakeAffineMatrix(uvTransform_.scale_, uvTransform_.rotate_, uvTransform_.translate_);
}