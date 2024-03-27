#include "Bone.h"

Bone ReadBone(aiBone* bone) {
	Bone result;
	result.name = bone->mName.C_Str();

	return result;
}