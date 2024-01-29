#pragma once
#include "DirectXGame/Manager/CollisionManager.h"
#include "DirectXGame/Manager/InputManager.h"
#include "DirectXGame/Object/Model.h"

class Stage;

class Player
{
public:
	Player();
	~Player();

	void Initialize();

	void Update();

	void Draw();

public: //ゲッターセッター

	inline bool IsDead() const { return isDead_; }
	inline void SetDead(bool isDead) { isDead_ = isDead; }
	inline void SetPlayerPos(Vector3 pos) { playerModelInfo_.worldTransform_.data_.translate_ = pos; }
	inline void SetVelocity(Vector3 velocity) { velocity_ = velocity; }
	inline void ParentStage(Stage* stage) { stage_ = stage; }

private:
	InputManager* input_ = nullptr;
	Stage* stage_ = nullptr;

	std::shared_ptr<Model> playerModel_;
	RenderItem playerModelInfo_;
	std::unique_ptr<Collider> collider_;
	Vector3 velocity_;
	bool isDead_;
};

