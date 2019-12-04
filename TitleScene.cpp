/**
* @file TitleScene.cpp
*/
#include "Src/pch.h"
#include "Src/TitleScene.h"
#include "Src/MainGameScene.h"

/**
* ƒvƒŒƒCƒ„[‚Ì“ü—Í‚ğˆ—‚·‚é.
*/
void TitleScene::ProcessInput()
{
	SceneStack::Instance().Replace(std::make_shared<MainGameScene>());
}