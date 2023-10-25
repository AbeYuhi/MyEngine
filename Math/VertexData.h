#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

struct VertexData final {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};