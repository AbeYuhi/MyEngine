#include "Material.h"

void Material::Initialize() {
	//色の書き込み
	color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	enableLightint = false;
	uvTransform = MakeIdentity4x4();
}