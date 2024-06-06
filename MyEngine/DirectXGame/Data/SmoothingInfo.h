#pragma once
#include <stdint.h>

struct SmoothingInfo {
	int32_t kernelSize;
	int32_t type;
	float blurStrength;
};