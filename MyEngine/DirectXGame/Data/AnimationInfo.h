#pragma once
#include <stdint.h>
#include <string>
#include "Node.h"

struct AnimationInfo final{
	uint32_t frame;
	std::string name;
	std::string preName;
	bool isAnimation;
	Node rootNode;

	void Initialize();

	void Update();
};