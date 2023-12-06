#pragma once
#include <random>

class RandomManager
{
public:
	static RandomManager* GetInstance();

	void Initialize();

public: //ゲッターセッター

	int GetRandomNumber(int min,int max);

	float GetRandomNumber(float min, float max);

private:
	RandomManager() = default;
	~RandomManager() = default;

private: //メンバ変数
	std::mt19937 randomEngine_;
};

