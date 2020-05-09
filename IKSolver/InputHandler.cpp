#include "InputHandler.h"

// Singelton.
InputHandler * InputHandler::instance = 0;
InputHandler* InputHandler::getInstance(GLFWwindow* window, IKSolver* IKSolver)
{
	if (instance == 0)
	{
		instance = new InputHandler(window, IKSolver);
	}

	return instance;
}

InputHandler::InputHandler(GLFWwindow* window, IKSolver* IKSolver) : m_IKSolver(IKSolver)
{
	m_currentX = 0;
	m_currentY = 0;

	glfwSetKeyCallback(window, keyCallbackStatic);
	glfwSetMouseButtonCallback(window, mouseCallbackStatic);
	glfwSetCursorPosCallback(window, cursorDraggingCallbackStatic);
}

//-------------------- Static Callbacks -----------------------//

void InputHandler::keyCallbackStatic(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	InputHandler* that = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
	instance->keyCallback(window, key, scancode, action, mods);
}

void InputHandler::mouseCallbackStatic(GLFWwindow* window, int button, int action, int mods)
{
	InputHandler* that = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
	instance->mouseCallback(window, button, action, mods);
}

void InputHandler::cursorDraggingCallbackStatic(GLFWwindow* window, double xpos, double ypos)
{
	InputHandler* that = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
	instance->cursorDraggingCallback(window, (float)xpos, (float)ypos);
}

void InputHandler::scrollCallbackStatic(GLFWwindow* window, double offsetX, double offsetY)
{
	InputHandler* that = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));

	instance->m_IKSolver->handleScrollCallback((float)offsetY);
}

//-------------------- Input Callbacks -----------------------//

void InputHandler::cursorDraggingCallback(GLFWwindow* window, float xpos, float ypos) { /*mouse draging, left for rotation and right mouse for translations*/

	float m_previousX = m_currentX;
	float m_previousY = m_currentY;
	m_currentX = xpos;
	m_currentY = ypos;

	float xrel = m_previousX - m_currentX;
	double yrel = m_previousY - m_currentY;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) 
	{
		m_IKSolver->handleLeftMouseDragging(m_currentX, m_previousX, m_currentY, m_previousY);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) 
	{
		// Convert from window coordinates to camera coordinates.
		float z = zFar + m_depth * (zNear - zFar);
		float m_transX = (float)(ASPECT_RATIO * xrel / (float)(DISPLAY_HEIGHT)* zNear * 2.0 * tan(fovy * M_PI / 360.0) * (zFar / z));
		float m_transY = (float)(ASPECT_RATIO * yrel / (float)(DISPLAY_WIDTH)* zNear * 2.0 * tan(fovy * M_PI / 360.0) * (zFar / z));

		m_IKSolver->handleRightMouseDragging(m_transX, m_transY);
	}
}

void InputHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	case GLFW_KEY_UP:
		m_IKSolver->handleArrowRotation(1, 1);
		break;

	case GLFW_KEY_DOWN:
		m_IKSolver->handleArrowRotation(1, -1);
		break;

	case GLFW_KEY_RIGHT:
		m_IKSolver->handleArrowRotation(0, 1);
		break;

	case GLFW_KEY_LEFT:
		m_IKSolver->handleArrowRotation(0, -1);
		break;

	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS) 
		{
			m_IKSolver->spacePressed();
		}
		break;
	}
}

void InputHandler::mouseCallback(GLFWwindow* window, int button, int action, int mods) { /*for clicking/choosing on objects in the scene*/
	double xpos, ypos;

	glfwGetCursorPos(window, &xpos, &ypos);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) || glfwSetScrollCallback(window, scrollCallbackStatic)) {
		
		m_depth;
		glReadPixels(xpos, DISPLAY_HEIGHT - ypos - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &m_depth);
		
		m_IKSolver->handleMouseCallback((float)xpos, (float)ypos);
	}
}

InputHandler::~InputHandler()
{
}
