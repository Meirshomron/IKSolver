#ifndef DISPLAY_INCLUDED_H
#define DISPLAY_INCLUDED_H
#define GLEW_STATIC
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Config.h"

using namespace Config;

class Display
{
public:
	Display();

	void Clear(float r, float g, float b, float a);
	void SwapBuffers();

	virtual ~Display();
//protected:
//private:
	void operator=(const Display& display) {}
	Display(const Display& display) {}

	GLFWwindow* m_window;
	int error;
};

#endif
