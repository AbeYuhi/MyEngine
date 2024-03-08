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

struct Animation {
	std::string name;
	uint32_t numFrames;
	AnimationChannnel channel;
};

struct PositionChannel {
	Vector3 position;
	float time;
};

struct RotationChannel {
	Vector4 rotation;
	float time;
};

struct ScaleChannel {
	Vector3 scale;
	float time;
};

struct AnimationChannnel {
	std::string name;
	PositionChannel positionChannel;
	RotationChannel rotationChannel;
	ScaleChannel scaleChannel;
};