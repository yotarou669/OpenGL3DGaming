/**
* @file MainGameScene.cpp
*/
#include "MainGameScene.h"
#include "StatusScene.h"
#include "GameOverScene.h"
#include "Gamepad.h"
#include "GLFWEW.h"


bool MainGameScene::Initialize()
{
	spriteRenderer.Init(1000, "Res/Sprite.vert", "Res/Sprite.frag");
	sprites.reserve(100);
	Sprite spr(Texture::Image2D::Create("Res/TitleBg.tga"));
	spr.Scale(glm::vec2(2));
	sprites.push_back(spr);
	return true;
}

/**
* �v���C���[�̓��͂���������.
*/
void MainGameScene::ProcessInput()
{

	GLFWEW::Window& window = GLFWEW::Window::Instance();
	if (window.GetGamePad().buttonDown&GamePad::START)
	{
		SceneStack::Instance().Replace(std::make_shared<StatusScene>());
	}
	else if(window.GetGamePad().buttonDown&GamePad::X)
	{
		SceneStack::Instance().Replace(std::make_shared<GameOverScene>());
	}
}

/*
//�V�[�����X�V����
//
//@oaram deltaTime �O��̍X�V����̌o�ߎ���(�b)
//*/
void MainGameScene::Update(float deltaTime)
{
	spriteRenderer.BeginUpdate();
	for (const Sprite& e : sprites)
	{
		spriteRenderer.AddVertices(e);
	}
	spriteRenderer.EndUpdate();
}


/*
�V�[����`�悷��
*/

void MainGameScene::Render()
{
	const GLFWEW::Window& window = GLFWEW::Window::Instance();
	const glm::vec2 screenSize(window.Width(), window.Height());
	spriteRenderer.Draw(screenSize);
}