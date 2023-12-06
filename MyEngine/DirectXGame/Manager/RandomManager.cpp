#include "RandomManager.h"

RandomManager* RandomManager::GetInstance() {
	static RandomManager instance;
	return &instance;
}

void RandomManager::Initialize() {
	//乱数生成器(シード)の初期化
	std::random_device seedGenerator;
	//シードの代入
	randomEngine_.seed(seedGenerator());
}

int RandomManager::GetRandomNumber(int min, int max) {
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(randomEngine_);
}

float RandomManager::GetRandomNumber(float min, float max) {
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(randomEngine_);
}
