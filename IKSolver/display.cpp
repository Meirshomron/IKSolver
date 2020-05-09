#include "display.h"

#include <iostream>


Display::Display()
{
	/* Initialize the library */
    if (!glfwInit())
        error =  -1;

	m_window = glfwCreateWindow(DISPLAY_WIDTH, DISPLAY_HEIGHT, windowTitle.c_str(), NULL, NULL);
	if(!m_window)
	{
		glfwTerminate();
		error = -1;
	}
	glfwMakeContextCurrent(m_window);
	//m_glContext = SDL_GL_CreateContext(m_window);

	GLenum res = glewInit();
    if(res != GLEW_OK)
   {
		std::cerr << "Glew failed to initialize!" << std::endl;
    }

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

Display::~Display()
{
	//SDL_GL_DeleteContext(m_glContext);
	glfwDestroyWindow(m_window);
	glfwTerminate();
	//SDL_Quit();
}

void Display::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::SwapBuffers()
{
	glfwSwapBuffers(m_window);
}
