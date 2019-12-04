/**
* @file MainGameScene.cpp
*/
#include "pch.h"
#include "MainGameScene.h"
#include "StatusScene.h"
#include "GameOverScene.h"

/**
* �v���C���[�̓��͂���������.
*/
void MainGameScene::ProcessInput()
{
	if (!flag) {
		flag = true;
		SceneStack::Instance().Push(std::make_shared<StatusScene>());
	}
	else {
		SceneStack::Instance().Replace(std::make_shared<GameOverScene>());
	}
}