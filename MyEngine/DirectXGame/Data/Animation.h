#pragma once
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Manager/ImGuiManager.h"
#include "Math/Math.h"
#include "AnimationData.h"
#include "Node.h"
#include "Skeleton.h"
#include "SkinCluster.h"

class Model;

struct Animation {
	//アニメーションのローカル情報
	std::list<AnimationInfo> infos;
	//Node情報
	Node rootNode;
	//初期モデルのNode
	Node initialNode;
	//Skeleton情報
	Skeleton skeleton;
	//SkinCluster情報
	std::map<std::string , SkinCluster> skinClusters;

	void Initialize();

	void Update();

	void NodeUpdate(AnimationInfo& info);

	void SkeletonUpdate();

	void ApplyAnimation(AnimationInfo& info);

	void SkinClusterUpdate();

	void SetAnimation(std::list<AnimationData> datas);

	void SkeletonDraw();

	void SetModel(Model* model);

	void SetAnimationSpeed(std::string animationName, float speed);

};