/**
* @file TitleScene.cpp
*/
#include "Src/pch.h"
#include "Src/TitleScene.h"
#include "Src/MainGameScene.h"

/**
* プレイヤーの入力を処理する.
*/
void TitleScene::ProcessInput()
{
	SceneStack::Instance().Replace(std::make_shared<MainGameScene>());
}