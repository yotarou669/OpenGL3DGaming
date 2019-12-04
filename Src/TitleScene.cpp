/**
* @file TitleScene.cpp
*/
#include "pch.h"
#include "TitleScene.h"
#include "MainGameScene.h"

/**
* ƒvƒŒƒCƒ„[‚Ì“ü—Í‚ğˆ—‚·‚é.
*/
void TitleScene::ProcessInput()
{
	SceneStack::Instance().Replace(std::make_shared<MainGameScene>());
}