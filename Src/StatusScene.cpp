/**
* @file StatusScene.cpp
*/
#include "pch.h"
#include "StatusScene.h"

/**
* プレイヤーの入力を処理する.
*/
void StatusScene::ProcessInput()
{
	SceneStack::Instance().Pop();
}