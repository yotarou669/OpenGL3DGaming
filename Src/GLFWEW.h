/*
`fale GLFWEW.h
*/
#ifndef GLFWEW_H_INCLUDED
#define GLFWEW_H_INCLUDED
#include <GL/glew.h>
#include "GamePad.h"
#include <GLFW/glfw3.h>

namespace GLFWEW
{

	/*
	GLFW‚ÆGLFWEW
	*/
	class Window
	{
	public:
		static Window&Instance();
		bool Init(int w, int h, const char* title);
		bool ShaouldClose() const;
		void SwapBuffers() const;

		void InitTimer();
		void Updatatimer();
		double DeltaTimer()const;
		int Width() const { return width; }
		int Height() const { return height; }
		bool IsKeyPressed(int key) const;
		const GamePad& GetGamePad() const;

	private:
		Window();
		~Window();
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		void UpdateGamePad();

		bool isGLFWInitiaLized = false;
		bool isInitialized = false;
		GLFWwindow* window = nullptr;
		int width = 0;
		int height = 0;
		double previousTime = 0;
		double deltaTime = 0;
		GamePad gamepad;
	};

}	//namespace GLFWEW

#endif	//GLFWEW_H_INCLUDED 
