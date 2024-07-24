#include "OBB.h"
#include "Sphere.h"

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
	AABB aabbLocal{ .min = {-obb.size.x, -obb.size.y, -obb.size.z}, .max{obb.size.x, obb.size.y, obb.size.z} };
	Sphere sphereOBBLocal{ .center = centerInOBBLocalSpace, .radius = sphere.radius };
	if (IsCollision(sphereOBBLocal, aabbLocal)) {
		return true;
	}
	return false;
}

bool IsCollision(const OBB& obb1, const OBB& obb2) {
	Vector3 separationAxis[15];

	Matrix4x4 rotate1Matrix = MakeIdentity4x4();
	rotate1Matrix.m[0][0] = obb1.orientations[0].x;
	rotate1Matrix.m[0][1] = obb1.orientations[0].y;
	rotate1Matrix.m[0][2] = obb1.orientations[0].z;

	rotate1Matrix.m[1][0] = obb1.orientations[1].x;
	rotate1Matrix.m[1][1] = obb1.orientations[1].y;
	rotate1Matrix.m[1][2] = obb1.orientations[1].z;

	rotate1Matrix.m[2][0] = obb1.orientations[2].x;
	rotate1Matrix.m[2][1] = obb1.orientations[2].y;
	rotate1Matrix.m[2][2] = obb1.orientations[2].z;

	rotate1Matrix.m[3][0] = obb1.center.x;
	rotate1Matrix.m[3][1] = obb1.center.y;
	rotate1Matrix.m[3][2] = obb1.center.z;

	Matrix4x4 rotate2Matrix = MakeIdentity4x4();
	rotate2Matrix.m[0][0] = obb2.orientations[0].x;
	rotate2Matrix.m[0][2] = obb2.orientations[0].y;
	rotate2Matrix.m[0][2] = obb2.orientations[0].z;

	rotate2Matrix.m[2][0] = obb2.orientations[2].x;
	rotate2Matrix.m[2][2] = obb2.orientations[2].y;
	rotate2Matrix.m[2][2] = obb2.orientations[2].z;

	rotate2Matrix.m[2][0] = obb2.orientations[2].x;
	rotate2Matrix.m[2][2] = obb2.orientations[2].y;
	rotate2Matrix.m[2][2] = obb2.orientations[2].z;

	rotate2Matrix.m[3][0] = obb2.center.x;
	rotate2Matrix.m[3][2] = obb2.center.y;
	rotate2Matrix.m[3][2] = obb2.center.z;

	Vector3 point1[8];

	point1[0] = { -obb1.size.x, -obb1.size.y, -obb1.size.z };

	point1[1] = { +obb1.size.x, -obb1.size.y, -obb1.size.z };

	point1[2] = { -obb1.size.x, +obb1.size.y, -obb1.size.z };

	point1[3] = { -obb1.size.x, -obb1.size.y, +obb1.size.z };

	point1[4] = { +obb1.size.x, +obb1.size.y, -obb1.size.z };

	point1[5] = { +obb1.size.x, -obb1.size.y, +obb1.size.z };

	point1[6] = { -obb1.size.x, +obb1.size.y, +obb1.size.z };

	point1[7] = { +obb1.size.x, +obb1.size.y, +obb1.size.z };

	for (int i = 0; i < 8; i++) {
		point1[i] = Transform(point1[i], rotate1Matrix);
	}

	Vector3 point2[8];

	point2[0] = { -obb2.size.x, -obb2.size.y, -obb2.size.z };

	point2[1] = { +obb2.size.x, -obb2.size.y, -obb2.size.z };

	point2[2] = { -obb2.size.x, +obb2.size.y, -obb2.size.z };

	point2[3] = { -obb2.size.x, -obb2.size.y, +obb2.size.z };

	point2[4] = { +obb2.size.x, +obb2.size.y, -obb2.size.z };

	point2[5] = { +obb2.size.x, -obb2.size.y, +obb2.size.z };

	point2[6] = { -obb2.size.x, +obb2.size.y, +obb2.size.z };

	point2[7] = { +obb2.size.x, +obb2.size.y, +obb2.size.z };

	for (int i = 0; i < 8; i++) {
		point2[i] = Transform(point2[i], rotate2Matrix);
	}

	separationAxis[0] = Transform({ 1.0f, 0.0f, 0.0f }, rotate1Matrix);
	separationAxis[1] = Transform({ 0.0f, 1.0f, 0.0f }, rotate1Matrix);
	separationAxis[2] = Transform({ 0.0f, 0.0f, 1.0f }, rotate1Matrix);
	separationAxis[3] = Transform({ 1.0f, 0.0f, 0.0f }, rotate2Matrix);
	separationAxis[4] = Transform({ 0.0f, 1.0f, 0.0f }, rotate2Matrix);
	separationAxis[5] = Transform({ 0.0f, 0.0f, 1.0f }, rotate2Matrix);

	separationAxis[6] = Cross(separationAxis[0], separationAxis[3]);
	separationAxis[7] = Cross(separationAxis[0], separationAxis[4]);
	separationAxis[8] = Cross(separationAxis[0], separationAxis[5]);
	separationAxis[9] = Cross(separationAxis[1], separationAxis[3]);
	separationAxis[10] = Cross(separationAxis[1], separationAxis[4]);
	separationAxis[11] = Cross(separationAxis[1], separationAxis[5]);
	separationAxis[12] = Cross(separationAxis[2], separationAxis[3]);
	separationAxis[13] = Cross(separationAxis[2], separationAxis[4]);
	separationAxis[14] = Cross(separationAxis[2], separationAxis[5]);

	for (int i = 0; i < 15; i++) {
		float max1 = 0.0f, max2 = 0.0f;
		float min1 = 0.0f, min2 = 0.0f;

		for (int j = 0; j < 8; j++) {
			float tmp = 0.0f;
			tmp = Dot(point1[j], Normalize(separationAxis[i]));
			if (j == 0 || min1 > tmp) {
				min1 = tmp;
			}
			if (j == 0 || max1 < tmp) {
				max1 = tmp;
			}

			tmp = Dot(point2[j], Normalize(separationAxis[i]));
			if (j == 0 || min2 > tmp) {
				min2 = tmp;
			}
			if (j == 0 || max2 < tmp) {
				max2 = tmp;
			}
		}

		float L1 = max1 - min1;
		float L2 = max2 - min2;

		float sumSpan = L1 + L2;
		float lonSpan = (std::max)(max1, max2) - (std::min)(min1, min2);
		if (sumSpan < lonSpan) {
			return false;
		}
	}
	return true;
}

bool IsCollision(const OBB& obb, const AABB& aabb) {
	Vector3 separationAxis[15];

	Matrix4x4 rotateMatrix = MakeIdentity4x4();
	for (int i = 0; i < 3; ++i) {
		rotateMatrix.m[i][0] = obb.orientations[i].x;
		rotateMatrix.m[i][1] = obb.orientations[i].y;
		rotateMatrix.m[i][2] = obb.orientations[i].z;
	}
	rotateMatrix.m[3][0] = obb.center.x;
	rotateMatrix.m[3][1] = obb.center.y;
	rotateMatrix.m[3][2] = obb.center.z;

	Vector3 aabbCenter = { (aabb.min.x + aabb.max.x) / 2.0f, (aabb.min.y + aabb.max.y) / 2.0f, (aabb.min.z + aabb.max.z) / 2.0f };
	Vector3 aabbHalfSize = { (aabb.max.x - aabb.min.x) / 2.0f, (aabb.max.y - aabb.min.y) / 2.0f, (aabb.max.z - aabb.min.z) / 2.0f };

	Vector3 point1[8] = {
		{-obb.size.x, -obb.size.y, -obb.size.z},
		{+obb.size.x, -obb.size.y, -obb.size.z},
		{-obb.size.x, +obb.size.y, -obb.size.z},
		{-obb.size.x, -obb.size.y, +obb.size.z},
		{+obb.size.x, +obb.size.y, -obb.size.z},
		{+obb.size.x, -obb.size.y, +obb.size.z},
		{-obb.size.x, +obb.size.y, +obb.size.z},
		{+obb.size.x, +obb.size.y, +obb.size.z}
	};

	for (int i = 0; i < 8; ++i) {
		point1[i] = Transform(point1[i], rotateMatrix);
	}

	Vector3 point2[8] = {
		{aabb.min.x, aabb.min.y, aabb.min.z},
		{aabb.max.x, aabb.min.y, aabb.min.z},
		{aabb.min.x, aabb.max.y, aabb.min.z},
		{aabb.min.x, aabb.min.y, aabb.max.z},
		{aabb.max.x, aabb.max.y, aabb.min.z},
		{aabb.max.x, aabb.min.y, aabb.max.z},
		{aabb.min.x, aabb.max.y, aabb.max.z},
		{aabb.max.x, aabb.max.y, aabb.max.z}
	};

	separationAxis[0] = { 1.0f, 0.0f, 0.0f };
	separationAxis[1] = { 0.0f, 1.0f, 0.0f };
	separationAxis[2] = { 0.0f, 0.0f, 1.0f };
	separationAxis[3] = Transform({ 1.0f, 0.0f, 0.0f }, rotateMatrix);
	separationAxis[4] = Transform({ 0.0f, 1.0f, 0.0f }, rotateMatrix);
	separationAxis[5] = Transform({ 0.0f, 0.0f, 1.0f }, rotateMatrix);

	separationAxis[6] = Cross(separationAxis[0], separationAxis[3]);
	separationAxis[7] = Cross(separationAxis[0], separationAxis[4]);
	separationAxis[8] = Cross(separationAxis[0], separationAxis[5]);
	separationAxis[9] = Cross(separationAxis[1], separationAxis[3]);
	separationAxis[10] = Cross(separationAxis[1], separationAxis[4]);
	separationAxis[11] = Cross(separationAxis[1], separationAxis[5]);
	separationAxis[12] = Cross(separationAxis[2], separationAxis[3]);
	separationAxis[13] = Cross(separationAxis[2], separationAxis[4]);
	separationAxis[14] = Cross(separationAxis[2], separationAxis[5]);

	for (int i = 0; i < 15; ++i) {
		float max1 = -std::numeric_limits<float>::infinity(), max2 = -std::numeric_limits<float>::infinity();
		float min1 = std::numeric_limits<float>::infinity(), min2 = std::numeric_limits<float>::infinity();

		for (int j = 0; j < 8; ++j) {
			float tmp = Dot(point1[j], Normalize(separationAxis[i]));
			min1 = std::min(min1, tmp);
			max1 = std::max(max1, tmp);

			tmp = Dot(point2[j], Normalize(separationAxis[i]));
			min2 = std::min(min2, tmp);
			max2 = std::max(max2, tmp);
		}

		float L1 = max1 - min1;
		float L2 = max2 - min2;

		float sumSpan = L1 + L2;
		float lonSpan = std::max(max1, max2) - std::min(min1, min2);
		if (sumSpan < lonSpan) {
			return false;
		}
	}

	return true;
}