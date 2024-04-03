#pragma once
#include <string>
#include <vector>
#include <list>
#include "AnimationData.h"
#include "Node.h"

struct Animation {
	//アニメーションのローカル情報
	std::list<AnimationInfo> infos;
	//Node情報
	Node rootNode;
	//初期モデルのNode
	Node initialNode;

	void Initialize();

	void Update();

	void NodeUpdate(AnimationInfo info);

	void SetAnimation(std::list<AnimationData> datas);

	inline void SetInialNode(Node node) { initialNode = node; }

};