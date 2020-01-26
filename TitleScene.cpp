/**
* @file TitleScene.cpp
*/
#include "Src/TitleScene.h"
#include "Src/MainGameScene.h"
#include "Src/GLFWEW.h"


/**
* �V�[��������������.
*
* @retval true  ����������.
* @retval false ���������s. �Q�[���i�s�s�ɂ��A�v���O�������I�����邱��.
*/
bool TitleScene::Initialize()
{
	spriteRenderer.Init(1000, "Res/Sprite.vert", "Res/Sprite.frag");
	sprites.reserve(100);
	Sprite spr(Texture::Image2D::Create("Res/TitleBg.tga"));
	spr.Scale(glm::vec2(2));
	sprites.push_back(spr);
	fontRenderer.Init(1000);
	fontRenderer.LoadFromFile("Res/font.fnt");
	return true;
}

/**
* �v���C���[�̓��͂���������.
*/
void TitleScene::ProcessInput()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	if (window.GetGamePad().buttonDown&GamePad::START)
	{
		SceneStack::Instance().Replace(std::make_shared<MainGameScene>());
	}

}


/**
* �V�[�����X�V����.
*
* @param deltaTime  �O��̍X�V����̌o�ߎ���(�b).
*/
void TitleScene::Update(float deltaTime)
 {
	spriteRenderer.BeginUpdate();
	for (const Sprite& e : sprites) {
	spriteRenderer.AddVertices(e);
	
	}
spriteRenderer.EndUpdate();


const GLFWEW::Window& window = GLFWEW::Window::Instance();
const float w = static_cast<float>(window.Width());
const float h = static_cast<float>(window.Height());
const float lineHeight = fontRenderer.LineHeight();
fontRenderer.BeginUpdate();
fontRenderer.AddString(glm::vec2(-w * 0.5f + 32, h * 0.5f - lineHeight), L"�^�C�g�����");
fontRenderer.AddString(glm::vec2(-128, 0), L"�A�N�V�����Q�[��");
fontRenderer.EndUpdate();
}


/**
* �V�[����`�悷��.
*/
void TitleScene::Render()
 {
const GLFWEW::Window& window = GLFWEW::Window::Instance();
const glm::vec2 screenSize(window.Width(), window.Height());
spriteRenderer.Draw(screenSize);
fontRenderer.Draw(screenSize);
}