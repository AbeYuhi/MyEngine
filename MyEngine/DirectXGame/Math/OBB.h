#pragma once
#include "Vector3.h"
#include "Sphere.h"
#include "Matrix4x4.h"
#include "Math.h"

struct OBB
{
	Vector3 center;
	Vector3 orientations[3];
	Vector3 size;
};

bool IsCollision(const OBB& obb, const Sphere& sphere);

bool IsCollision(const Sphere& sphere, const OBB& obb);

bool OverlapOnAxis(const OBB& obb1, const OBB& obb2, const Vector3& axis);

bool IsCollision(const OBB& obb1, const OBB& obb2);

bool IsCollision(const OBB& obb, const AABB& aabb);

bool IsCollision(const AABB& aabb, const OBB& obb);

