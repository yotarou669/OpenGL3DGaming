/**
* @file TitleScene.cpp
*/
#include "pch.h"
#include "TitleScene.h"
#include "MainGameScene.h"

/**
* �v���C���[�̓��͂���������.
*/
void TitleScene::ProcessInput()
{
	SceneStack::Instance().Replace(std::make_shared<MainGameScene>());
}