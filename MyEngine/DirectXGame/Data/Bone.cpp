#include "Bone.h"

Bone ReadBone(aiBone* bone) {
	Bone result;
	result.name = bone->mName.C_Str();
	result.node = ReadNode(bone->mNode);


	return result;
}