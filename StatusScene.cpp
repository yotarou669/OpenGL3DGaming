/**
* @file StatusScene.cpp
*/
#include "Src/pch.h"
#include "Src/StatusScene.h"

/**
* プレイヤーの入力を処理する.
*/
void StatusScene::ProcessInput()
{
	SceneStack::Instance().Pop();
}