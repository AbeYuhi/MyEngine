#include "OBB.h"

bool IsCollision(const OBB& obb, const Sphere& sphere) {
	Matrix4x4 worldMatrix = MakeIdentity4x4();
	worldMatrix.m[0][0] = obb.orientations[0].x;
	worldMatrix.m[0][1] = obb.orientations[0].y;
	worldMatrix.m[0][2] = obb.orientations[0].z;

	worldMatrix.m[1][0] = obb.orientations[1].x;
	worldMatrix.m[1][1] = obb.orientations[1].y;
	worldMatrix.m[1][2] = obb.orientations[1].z;

	worldMatrix.m[2][0] = obb.orientations[2].x;
	worldMatrix.m[2][1] = obb.orientations[2].y;
	worldMatrix.m[2][2] = obb.orientations[2].z;

	worldMatrix.m[3][0] = obb.center.x;
	worldMatrix.m[3][1] = obb.center.y;
	worldMatrix.m[3][2] = obb.center.z;

	Matrix4x4 worldMatrixInverse = Inverse(worldMatrix);
	Vector3 centerInOBBLocalSpace = Transform(sphere.center, worldMatrixInverse);
	AABB aabbLocal{ .min = {-obb.size.x / 2.0f, -obb.size.y / 2.0f, -obb.size.z / 2.0f}, .max{obb.size.x / 2.0f, obb.size.y / 2.0f, obb.size.z / 2.0f} };
	Sphere sphereOBBLocal{ .center = centerInOBBLocalSpace, .radius = sphere.radius };
	if (IsCollision(sphereOBBLocal, aabbLocal)) {
		return true;
	}
	return false;
}

bool IsCollision(const Sphere& sphere, const OBB& obb) {
	Matrix4x4 worldMatrix = MakeIdentity4x4();
	worldMatrix.m[0][0] = obb.orientations[0].x;
	worldMatrix.m[0][1] = obb.orientations[0].y;
	worldMatrix.m[0][2] = obb.orientations[0].z;

	worldMatrix.m[1][0] = obb.orientations[1].x;
	worldMatrix.m[1][1] = obb.orientations[1].y;
	worldMatrix.m[1][2] = obb.orientations[1].z;

	worldMatrix.m[2][0] = obb.orientations[2].x;
	worldMatrix.m[2][1] = obb.orientations[2].y;
	worldMatrix.m[2][2] = obb.orientations[2].z;

	worldMatrix.m[3][0] = obb.center.x;
	worldMatrix.m[3][1] = obb.center.y;
	worldMatrix.m[3][2] = obb.center.z;

	Matrix4x4 worldMatrixInverse = Inverse(worldMatrix);
	Vector3 centerInOBBLocalSpace = Transform(sphere.center, worldMatrixInverse);
	AABB aabbLocal{ .min = {-obb.size.x / 2.0f, -obb.size.y / 2.0f, -obb.size.z / 2.0f}, .max{obb.size.x / 2.0f, obb.size.y / 2.0f, obb.size.z / 2.0f} };
	Sphere sphereOBBLocal{ .center = centerInOBBLocalSpace, .radius = sphere.radius };
	if (IsCollision(sphereOBBLocal, aabbLocal)) {
		return true;
	}
	return false;
}

bool OverlapOnAxis(const OBB& obb1, const OBB& obb2, const Vector3& axis) {
	// Project both OBBs onto the axis and check for overlap
	float obb1Projection =
		fabs(Dot(obb1.orientations[0], axis)) * (obb1.size.x / 2.0f) +
		fabs(Dot(obb1.orientations[1], axis)) * (obb1.size.y / 2.0f) +
		fabs(Dot(obb1.orientations[2], axis)) * (obb1.size.z / 2.0f);
												
	float obb2Projection =						
		fabs(Dot(obb2.orientations[0], axis)) * (obb2.size.x / 2.0f) +
		fabs(Dot(obb2.orientations[1], axis)) * (obb2.size.y / 2.0f) +
		fabs(Dot(obb2.orientations[2], axis)) * (obb2.size.z / 2.0f);

	float distance = fabs(Dot((obb2.center - obb1.center), axis));

	return distance <= obb1Projection + obb2Projection;
}

bool IsCollision(const OBB& obb1, const OBB& obb2) {
	// Test the 6 axes of the two OBBs
	for (int i = 0; i < 3; ++i) {
		if (!OverlapOnAxis(obb1, obb2, obb1.orientations[i])) {
			return false;
		}
		if (!OverlapOnAxis(obb1, obb2, obb2.orientations[i])) {
			return false;
		}
	}

	// Test the 9 cross products of the axes
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			Vector3 axis = Cross(obb1.orientations[i], obb2.orientations[j]);
			if (axis.x != 0 || axis.y != 0 || axis.z != 0) { // Check for zero vector
				axis = Normalize(axis);
				if (!OverlapOnAxis(obb1, obb2, axis)) {
					return false;
				}
			}
		}
	}

	return true; // No separating axis found, OBBs must be intersecting
}

bool IsCollision(const OBB& obb1, const AABB& aabb) {

	Vector3 aabbCenter = { (aabb.min.x + aabb.max.x) / 2.0f, (aabb.min.y + aabb.max.y) / 2.0f, (aabb.min.z + aabb.max.z) / 2.0f };

	OBB obb2;
	obb2.center = aabbCenter;
	Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3{ 0, 0, 0 });
	obb2.orientations[0].x = rotateMatrix.m[0][0];
	obb2.orientations[0].y = rotateMatrix.m[0][1];
	obb2.orientations[0].z = rotateMatrix.m[0][2];

	obb2.orientations[1].x = rotateMatrix.m[1][0];
	obb2.orientations[1].y = rotateMatrix.m[1][1];
	obb2.orientations[1].z = rotateMatrix.m[1][2];
	
	obb2.orientations[2].x = rotateMatrix.m[2][0];
	obb2.orientations[2].y = rotateMatrix.m[2][1];
	obb2.orientations[2].z = rotateMatrix.m[2][2];

	obb2.size = { aabb.max.x - aabb.min.x, aabb.max.y - aabb.min.y , aabb.max.z - aabb.min.z };

	return IsCollision(obb1, obb2);
}

bool IsCollision(const AABB& aabb, const OBB& obb1) {

	Vector3 aabbCenter = { (aabb.min.x + aabb.max.x) / 2.0f, (aabb.min.y + aabb.max.y) / 2.0f, (aabb.min.z + aabb.max.z) / 2.0f };

	OBB obb2;
	obb2.center = aabbCenter;
	Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3{ 0, 0, 0 });
	obb2.orientations[0].x = rotateMatrix.m[0][0];
	obb2.orientations[0].y = rotateMatrix.m[0][1];
	obb2.orientations[0].z = rotateMatrix.m[0][2];

	obb2.orientations[1].x = rotateMatrix.m[1][0];
	obb2.orientations[1].y = rotateMatrix.m[1][1];
	obb2.orientations[1].z = rotateMatrix.m[1][2];

	obb2.orientations[2].x = rotateMatrix.m[2][0];
	obb2.orientations[2].y = rotateMatrix.m[2][1];
	obb2.orientations[2].z = rotateMatrix.m[2][2];

	obb2.size = { aabb.max.x - aabb.min.x, aabb.max.y - aabb.min.y , aabb.max.z - aabb.min.z };

	return IsCollision(obb1, obb2);
}