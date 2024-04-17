#include "AnimationData.h"

Vector3 CalculateValue(const AnimationCurve<Vector3>& curve, float time) {
	assert(!curve.keyframes.empty());
	if (curve.keyframes.size() == 1 || time <= curve.keyframes[0].time) {
		return curve.keyframes[0].value;
	}

	for (size_t index = 0; index < curve.keyframes.size() - 1; index++) {
		size_t nextIndex = index + 1;
		//indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判定
		if (curve.keyframes[index].time <= time && time <= curve.keyframes[nextIndex].time) {
			//範囲内を補間
			float t = (time - curve.keyframes[index].time) / (curve.keyframes[nextIndex].time - curve.keyframes[index].time);
			return Lerp(curve.keyframes[index].value, curve.keyframes[nextIndex].value, t);
		}
	}

	return curve.keyframes.rbegin()->value;
}

Quaternion CalculateValue(const AnimationCurve<Quaternion>& curve, float time) {
	assert(!curve.keyframes.empty());
	if (curve.keyframes.size() == 1 || time <= curve.keyframes[0].time) {
		return curve.keyframes[0].value;
	}

	for (size_t index = 0; index < curve.keyframes.size() - 1; index++) {
		size_t nextIndex = index + 1;
		//indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判定
		if (curve.keyframes[index].time <= time && time <= curve.keyframes[nextIndex].time) {
			//範囲内を補間
			float t = (time - curve.keyframes[index].time) / (curve.keyframes[nextIndex].time - curve.keyframes[index].time);
			return Slerp(curve.keyframes[index].value, curve.keyframes[nextIndex].value, t);
		}
	}

	return curve.keyframes.rbegin()->value;
}
