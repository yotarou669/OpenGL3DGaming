/**
* @file MainGameScene.cpp
*/
#include "Src/pch.h"
#include "Src/MainGameScene.h"
#include "Src/StatusScene.h"
#include "Src/GameOverScene.h"

/**
* プレイヤーの入力を処理する.
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