/**
* @file StatusScene.cpp
*/
#include "Src/pch.h"
#include "Src/StatusScene.h"

/**
* �v���C���[�̓��͂���������.
*/
void StatusScene::ProcessInput()
{
	SceneStack::Instance().Pop();
}