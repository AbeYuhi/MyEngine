#include "ObjectManager.h"

ObjectManager* ObjectManager::GetInstance() {
	static ObjectManager instance;
	return &instance;
}

Model* ObjectManager::Create(const std::string& fileName) {
	return GetInstance()->LoadObj(fileName);
}

Model* ObjectManager::LoadObj(const std::string& fileName) {
	if (modelDatas_.find(fileName) == modelDatas_.end()) {
		modelDatas_[fileName] = Model::Create(fileName);
	}

	return modelDatas_[fileName].get();
}

WireFrameBox* ObjectManager::CreateWireFrameBox() {
	return GetInstance()->LoadWireFrameBox();
}

WireFrameBox* ObjectManager::LoadWireFrameBox() {

	if (!wireFrameBoxData_) {
		wireFrameBoxData_ = WireFrameBox::Create();
	}

	return wireFrameBoxData_.get();
}

WireFrameSpriteBox* ObjectManager::CreateWireFrameSpriteBox() {
	return GetInstance()->LoadWireFrameSpriteBox();
}

WireFrameSpriteBox* ObjectManager::LoadWireFrameSpriteBox() {

	if (!wireFrameSpriteBoxData_) {
		wireFrameSpriteBoxData_ = WireFrameSpriteBox::Create();
	}

	return wireFrameSpriteBoxData_.get();
}

Sprite* ObjectManager::CreateSprite() {
	return GetInstance()->LoadSprite();
}

Sprite* ObjectManager::LoadSprite() {
	if (!spriteData_) {
		spriteData_ = Sprite::Create();
	}

	return spriteData_.get();
}