/**
* @file GameOverScene.cpp
*/
#include "Src/pch.h"
#include "Src/GameOverScene.h"
#include "Src/TitleScene.h"

/**
* プレイヤーの入力を処理する.
*/
void GameOverScene::ProcessInput()
{
	SceneStack::Instance().Replace(std::make_shared<TitleScene>());
}