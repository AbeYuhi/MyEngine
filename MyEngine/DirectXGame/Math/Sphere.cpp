#include "Sphere.h"
#define M_PI 3.14f

bool IsCollision(const Sphere& s1, const Sphere& s2) {
	auto distance = Length(s2.center - s1.center);

	if (distance <= s1.radius + s2.radius) {
		return true;
	}

	return false;
}

bool IsCollision(const Sphere& sphere, const AABB& aabb) {
	Vector3 closestPoint = {
		std::clamp(sphere.center.x, aabb.min.x, aabb.max.x),
		std::clamp(sphere.center.y, aabb.min.y, aabb.max.y),
		std::clamp(sphere.center.z, aabb.min.z, aabb.max.z),
	};

	float distance = Length(closestPoint - sphere.center);

	if (distance <= sphere.radius) {
		return true;
	}

	return false;
}

bool IsCollision(const AABB& aabb, const Sphere& sphere) {
	Vector3 closestPoint = {
		std::clamp(sphere.center.x, aabb.min.x, aabb.max.x),
		std::clamp(sphere.center.y, aabb.min.y, aabb.max.y),
		std::clamp(sphere.center.z, aabb.min.z, aabb.max.z),
	};

	float distance = Length(closestPoint - sphere.center);

	if (distance <= sphere.radius) {
		return true;
	}

	return false;
}