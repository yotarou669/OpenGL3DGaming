/**
* @file StatusScene.cpp
*/
#include "pch.h"
#include "StatusScene.h"

/**
* �v���C���[�̓��͂���������.
*/
void StatusScene::ProcessInput()
{
	SceneStack::Instance().Pop();
}