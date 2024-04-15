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

struct PositionChannel {
	Vector3 position;
	float time;
};

struct RotationChannel {
	Quaternion rotation;
	float time;
};

struct ScaleChannel {
	Vector3 scale;
	float time;
};

struct NodeAnimation {
	std::string name;
	std::vector<KeyframeVector3> position;
	std::vector<KeyframeQuaternion> rotation;
	std::vector<KeyframeVector3> scale;
};

struct AnimationData {
	std::string name;
	uint32_t numFrames;
	float duration;
	uint32_t numChannels;
	std::vector<NodeAnimation> animations;
};

struct AnimationInfo {
	AnimationData data;
	bool isAnimation;
	bool preIsAnimation;
	uint32_t frame;
};