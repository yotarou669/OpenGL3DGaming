/**
* @file GameOverScene.cpp
*/
#include "Src/GameOverScene.h"
#include "Src/TitleScene.h"
#include "Src/GLFWEW.h"

/**
* プレイヤーの入力を処理する.
*/
void GameOverScene::ProcessInput()
{
	SceneStack::Instance().Replace(std::make_shared<TitleScene>());

	GLFWEW::Window& window = GLFWEW::Window::Instance();
	if (window.GetGamePad().buttonDown&GamePad::START)
	{
		SceneStack::Instance().Replace(std::make_shared<TitleScene>());
	}
}