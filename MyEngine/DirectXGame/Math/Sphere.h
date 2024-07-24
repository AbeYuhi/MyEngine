#pragma once
#include <math.h>
#include <algorithm>
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix4x4.h"
#include "Math.h"
#include "AABB.h"

struct Sphere {
	Vector3 center;
	float radius;
};

bool IsCollision(const Sphere& s1, const Sphere& s2);

bool IsCollision(const Sphere& sphere, const AABB& aabb);

bool IsCollision(const AABB& aabb, const Sphere& sphere);
