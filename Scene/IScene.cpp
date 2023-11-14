#include "Scene/IScene.h"

int IScene::sceneNo_;
#ifdef _DEBUG
int IScene::debugMode_ = true;
#else
int IScene::debugMode_ = false;
#endif // _DEBUG
