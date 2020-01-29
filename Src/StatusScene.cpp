/**
* @file StatusScene.cpp
*/
#include "Scene.h"
#include "StatusScene.h"
#include <vector>
#include "GLFWEW.h"


bool StatusScene::Initialize()
{
	fontRenderer.Init(1000);
	fontRenderer.LoadFromFile("Res/font.fnt");
	return true;
}


/**
* プレイヤーの入力を処理する.
*/
void StatusScene::ProcessInput()
{
	SceneStack::Instance().Pop();
}



void StatusScene::Update(float)
{
	const GLFWEW ::Window& window = GLFWEW::Window::Instance();
	const float w = static_cast<float>(window.Width());
	const float h = static_cast<float>(window.Height());
	const float lineHeight = fontRenderer.LineHeight();
	fontRenderer.BeginUpdate();
	fontRenderer.AddString(glm::vec2(-w * 0.5f + 32, h * 0.5f - lineHeight), L"タイトル画面");
	fontRenderer.AddString(glm::vec2(-128, 0), L"アクションゲーム");
	fontRenderer.EndUpdate();
}


void StatusScene::Render()
{

	const GLFWEW::Window& window = GLFWEW::Window::Instance();
	const glm::vec2 screenSize(window.Width(), window.Height());
	fontRenderer.Draw(screenSize);
}

void StatusScene::Finalize()
{

}