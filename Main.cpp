// OpenGL3DGaming.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#include <Windows.h>
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

		//ESCキーが押されたら終了ウィンドウを表示
		if (window.IsKeyPressed(GLFW_KEY_ESCAPE))
		{
			if (MessageBox(nullptr, L"ゲームを終了しますか？",L"終了", MB_OKCANCEL) == IDOK)
			{
				break;
			}
		}
		const float deltaTime = static_cast<float>(window.DeltaTimer());
		sceneStack.Update(deltaTime);

		//バックアップ消去する
		glClearColor(0.8f, 0.2f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//GLコンテキストのパラメータ
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		sceneStack.Render();
		window.SwapBuffers();
		
	}
}

