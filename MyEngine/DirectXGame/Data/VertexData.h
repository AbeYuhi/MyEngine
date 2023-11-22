#pragma once
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/Vector4.h"

struct VertexData final {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};