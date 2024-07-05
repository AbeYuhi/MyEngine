#include "Skeleton.h"

int32_t CreateJoint(const RootNode& rootNode, const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints) {
	Joint joint;
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.skeletonSpaceMatrix = MakeIdentity4x4();
	joint.transform = node.transform;
	joint.index = int32_t(joints.size());
	joint.parent = parent;
	joints.push_back(joint);

	for (uint32_t index = 0; index < node.children.size(); index++) {
		int32_t childIndex = CreateJoint(rootNode, rootNode.nodes[node.children[index]], joint.index, joints);
		joints[joint.index].children.push_back(childIndex);
	}

	//自身のindexを返す
	return joint.index;
}

Skeleton CreateSkeleton(const RootNode& rootNode) {
	Skeleton skeleton;
	skeleton.root = CreateJoint(rootNode, rootNode.nodes[rootNode.rootNode], {}, skeleton.joints);

	//名前とindexのマッピングを行いアクセスしやすくする
	for (const Joint& joint : skeleton.joints) {
		skeleton.jointMap.emplace(joint.name, joint.index);
	}

	//行列の更新
	for (Joint& joint : skeleton.joints) {
		joint.localMatrix = MakeAffineMatrix(joint.transform.scale_, Normalize(joint.transform.rotate_), joint.transform.translate_);
		if (joint.parent) {
			joint.skeletonSpaceMatrix = joint.localMatrix * skeleton.joints[*joint.parent].skeletonSpaceMatrix;
		}
		else {
			joint.skeletonSpaceMatrix = joint.localMatrix;
		}
	}

	return skeleton;
}