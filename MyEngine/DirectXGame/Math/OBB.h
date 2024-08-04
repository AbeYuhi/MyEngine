#pragma once
#include "Vector3.h"
#include "Sphere.h"
#include "Matrix4x4.h"

struct OBB
{
	Vector3 center;
	Vector3 orientations[3];
	Vector3 size;
};

