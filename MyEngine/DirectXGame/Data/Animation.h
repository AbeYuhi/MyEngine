#pragma once
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include "Math/Matrix4x4.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

struct PositionChannel {
	Vector3 position;
	float time;
};

struct RotationChannel {
	Vector3 rotation;
	float time;
};

struct ScaleChannel {
	Vector3 scale;
	float time;
};

struct AnimationChannel {
	std::string name;
	uint32_t numPositionChannel;
	uint32_t numRotateChannel;
	uint32_t numScaleChannel;
	std::vector<PositionChannel> positionChannel;
	std::vector<RotationChannel> rotationChannel;
	std::vector<ScaleChannel> scaleChannel;
};

struct AnimationData {
	std::string name;
	uint32_t numFrames;
	uint32_t numChannels;
	std::vector<AnimationChannel> channels;
};