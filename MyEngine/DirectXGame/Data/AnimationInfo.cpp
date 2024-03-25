#include "AnimationInfo.h"


void AnimationInfo::Initialize() {
	name = "None";
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
	else {
		frame = 0;
	}

	preName = name;
}
