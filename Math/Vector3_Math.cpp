#include "Vector3_Math.hpp"

Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 v3 = {0, 0, 0};

	v3.x = v1.x + v2.x;

	v3.y = v1.y + v2.y;
	
	v3.z = v1.z + v2.z;

	return v3;
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 v3 = { 0, 0, 0 };

	v3.x = v1.x - v2.x;

	v3.y = v1.y - v2.y;

	v3.z = v1.z - v2.z;

	return v3;
}

Vector3 Multiply(float scalar, const Vector3& v) {
	Vector3 v3 = { 0, 0, 0 };

	v3.x = scalar * v.x;
 
	v3.y = scalar * v.y;
 
	v3.z = scalar * v.z;

	return v3;
}

float Dot(const Vector3& v1, const Vector3& v2) {
	float dot = 0.0f;

	dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return dot;
}

float Length(const Vector3& v) {
	float length = 0;

	length = sqrtf(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2));

	return length;
}

Vector3 Normalize(const Vector3& v1) {
	Vector3 v2 = { 0, 0, 0 };
	float length = Length(v1);

	v2.x = v1.x / length;
	v2.y = v1.y / length;
	v2.z = v1.z / length;

	return v2;
}

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 v3{};
	v3.x = v1.y * v2.z - v1.z * v2.y;
	v3.y = v1.z * v2.x - v1.x * v2.z;
	v3.z = v1.x * v2.y - v1.y * v2.x;

	return v3;
}

bool IsFront(const Vector3& v1, const Vector3 obj[3]) {

	Vector3 vecA = obj[1] - obj[0];
	Vector3 vecB = obj[2] - obj[1];

	Vector3 v2 = Cross(vecA, vecB);

	float dot = Dot(v1, v2);

	if (dot <= 0) {
		return true;
	}
	return false;
}

Vector3 Project(const Vector3& v1, const Vector3& v2) {
	float scalar = Dot(v1, Normalize(v2));

	return Multiply(scalar, Normalize(v2));
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
	Vector3 cp = segment.origin + project;
	return cp;
}

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]
	};
	return result;
}

Vector3 Perpendicular(const Vector3& v) {
	if (v.x != 0.0f || v.y != 0.0f) {
		return{ -v.y, v.x, 0.0f };
	}
	return{ 0.0f, -v.z, v.y };
}