#pragma once

#include "Config.h"
#include "IKSolver.h"
#include <GLFW/glfw3.h>

using namespace Config;

class InputHandler
{
	public:
		/* Static wrapper methods. */
		static InputHandler* getInstance(GLFWwindow* window, IKSolver* IKSolver);
		static void keyCallbackStatic(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouseCallbackStatic(GLFWwindow* window, int button, int action, int mods);
		static void cursorDraggingCallbackStatic(GLFWwindow* window, double xpos, double ypos);
		static void scrollCallbackStatic(GLFWwindow * window, double xoffset, double yoffset);

		~InputHandler();
	private:

		InputHandler(GLFWwindow* window, IKSolver* IKSolver);

		void mouseCallback(GLFWwindow* window, int button, int action, int mods);
		void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void cursorDraggingCallback(GLFWwindow* window, float xpos, float ypos);

		/* Here will be the instance stored. */
		static InputHandler* instance;
		IKSolver* m_IKSolver;
		
		float m_currentX;
		float m_currentY;

		float m_depth;
};

