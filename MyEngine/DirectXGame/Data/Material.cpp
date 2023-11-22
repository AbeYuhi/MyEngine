#include "Material.h"

void Material::Initialize() {
	//色の書き込み
	color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	enableLightint = false;
	isSpecularReflection = false;
	uvTransform = MakeIdentity4x4();
	shininess = 50.0f;
	shininessColor = { 1.0f, 1.0f, 1.0f };
}

void ParticleMaterial::Initialize() {
	//色の書き込み
	color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	uvTransform = MakeIdentity4x4();
}