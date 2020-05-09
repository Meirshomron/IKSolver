
#include <Windows.h>
#include "IKSolver.h"
#include "InputHandler.h"

int main(int argc, char** argv)
{
	Display display;
	IKSolver iKSolver;
	InputHandler* inputHandler = InputHandler::getInstance(display.m_window, &iKSolver);

	// Draw loop.
	while (!glfwWindowShouldClose(display.m_window))
	{
		Sleep(10);
		display.Clear(1.0f, 1.0f, 1.0f, 1.0f);

		iKSolver.draw();
		
		display.SwapBuffers();
		glfwPollEvents();
	}
	return 0;
}