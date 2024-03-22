#include "AnimationInfo.h"


void AnimationInfo::Initialize() {
	frame = 0;
	isAnimation = false;
}

void AnimationInfo::Update() {

	if (isAnimation) {
		if (name == preName) {
			frame++;
		}
		else {
			frame = 0;
		}
	}

	preName = name;
}
