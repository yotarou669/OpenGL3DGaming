// OpenGL3DGaming.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#include "Src/GLFWEW.h"
#include "Src/pch.h"
#include "Src/TitleScene.h"
#include <iostream>


int main()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	window.Init(1280, 780, u8"アクションゲーム");
	SceneStack & sceneStack = SceneStack::Instance();
	sceneStack.Push(std::make_shared<TitleScene>());
	

	while(!window.ShaouldClose())
	{
		window.Updatatimer();
		const float deltaTime = static_cast<float>(window.DeltaTimer());
		sceneStack.Update(deltaTime);
		sceneStack.Render();
		window.SwapBuffers();
		
	}
}

