#pragma once
#include <string>
#include <vector>
#include "Math/Matrix4x4.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Quaternion.h"
#include "Math/Math.h"

template <typename tValue>
struct Keyframe {
	tValue value;
	float time;
};

using KeyframeVector3 = Keyframe<Vector3>;
using KeyframeQuaternion = Keyframe<Quaternion>;

template <typename tValue>
struct AnimationCurve {
	std::vector<Keyframe<tValue>> keyframes;
};

struct NodeAnimation {
	std::string nodeName;
	AnimationCurve<Vector3> position;
	AnimationCurve<Quaternion> rotation;
	AnimationCurve<Vector3> scale;
};

struct AnimationData {
	std::string name;
	float duration;
	std::vector<NodeAnimation> nodeAnimations;
};

struct AnimationInfo {
	AnimationData data;
	bool isAnimation;
	bool preIsAnimation;
	float animationTime;
	float animationSpeed;
};

Vector3 CalculateValue(const AnimationCurve<Vector3>& curve, float time);

Quaternion CalculateValue(const AnimationCurve<Quaternion>& curve, float time);
