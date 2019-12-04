// OpenGL3DGaming.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"
#include "TitleScene.h"
#include <iostream>


int main()
{
	SceneStack & sceneStack = SceneStack::Instance();
	sceneStack.Push(std::make_shared<TitleScene>());
	
		for (;;) {
		const float deltaTime = 1.0f / 60.0f;
		sceneStack.Update(deltaTime);
		sceneStack.Render();
		
	}
}

