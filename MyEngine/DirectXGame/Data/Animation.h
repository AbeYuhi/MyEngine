#pragma once
#include <string>
#include <vector>
#include <list>
#include "Manager/ImGuiManager.h"
#include "AnimationData.h"
#include "Node.h"
#include "Skeleton.h"

struct Animation {
	//アニメーションのローカル情報
	std::list<AnimationInfo> infos;
	//Node情報
	Node rootNode;
	//初期モデルのNode
	Node initialNode;
	//Skeleton情報
	Skeleton skeleton;

	void Initialize();

	void Update();

	void NodeUpdate(AnimationInfo& info);

	void SkeletonUpdate();

	void SetAnimation(std::list<AnimationData> datas);

	inline void SetInialNode(Node node) { initialNode = node; }

	void SetAnimationSpeed(std::string animationName, float speed);

};