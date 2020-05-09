#ifndef SHADER_INCLUDED_H
#define SHADER_INCLUDED_H

#include "stb_image.h"
#include <string>
#include "glm\glm.hpp"

class Shader
{
public:
	Shader(const std::string& fileName);

	void Bind();
	void bindTexture(unsigned int index);
	void Update(glm::mat4 MVP, glm::mat4 Normal, int i = 0);
	unsigned int Texture(char const *filename);

	virtual ~Shader();
protected:
private:
	static const unsigned int NUM_SHADERS = 2;
	static const unsigned int NUM_UNIFORMS = 6;
	void operator=(const Shader& shader) {}
	Shader(const Shader& shader) {}
	
	std::string LoadShader(const std::string& fileName);
	void CheckShaderError(unsigned int shader, unsigned int flag, bool isProgram, const std::string& errorMessage);
	unsigned int CreateShader(const std::string& text, unsigned int type);

	unsigned int m_program;
	unsigned int m_shaders[NUM_SHADERS];
	unsigned int m_uniforms[NUM_UNIFORMS];
};

#endif
