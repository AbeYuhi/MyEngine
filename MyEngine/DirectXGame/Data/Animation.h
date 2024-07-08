#pragma once
#include <string>
#include <vector>
#include <list>
#include <map>
#include <chrono>
#include "Math/Math.h"
#include "AnimationData.h"
#include "Node.h"
#include "Skeleton.h"
#include "SkinCluster.h"

class Model;

struct Animation {
	//アニメーションのローカル情報
	std::vector<AnimationInfo> infos;
	//Node情報
	RootNode rootNode;
	//初期モデルのNode
	RootNode initialNode;
	//Skeleton情報
	Skeleton skeleton;
	Skeleton initialSkeleton;
	//SkinCluster情報
	std::map<std::string , SkinCluster> skinClusters;
	std::vector<std::string> meshNames;
	std::chrono::steady_clock::time_point preTime;

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