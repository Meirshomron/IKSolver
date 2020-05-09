#define GLEW_STATIC
#include <GL\glew.h>
#include "shader.h"
#include <iostream>
#include <fstream>



Shader::Shader(const std::string& fileName)
{
	
	m_program = glCreateProgram();
	m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	m_shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

	for(unsigned int i = 0; i < NUM_SHADERS; i++)
		glAttachShader(m_program, m_shaders[i]);

	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "texCoord");
	glBindAttribLocation(m_program, 2, "normal");
	glBindAttribLocation(m_program, 3, "color");


	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error linking shader program");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Invalid shader program");

	m_uniforms[0] = glGetUniformLocation(m_program, "MVP");
	m_uniforms[1] = glGetUniformLocation(m_program, "Normal");
	m_uniforms[2] = glGetUniformLocation(m_program, "lightDirection");
	m_uniforms[3] = glGetUniformLocation(m_program, "lightColor");
	m_uniforms[4] = glGetUniformLocation(m_program, "picking_color");
	m_uniforms[5] = glGetUniformLocation(m_program, "texture0");
}


Shader::~Shader()
{
	for(unsigned int i = 0; i < NUM_SHADERS; i++)
    {
        glDetachShader(m_program, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }

	glDeleteProgram(m_program);
}

void Shader::Bind()
{
	glUseProgram(m_program);
}

void Shader::bindTexture(unsigned int index)
{	
	// Activate the texture unit first before binding texture.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, index);
	glUniform1i(m_uniforms[5], 0);
}

unsigned int Shader::Texture(char const *filename) {

	GLuint texId;

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	// Set the texture wrapping / filtering options (on the currently bound texture object).
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	int textureSize, numComponents;
	unsigned char* image = stbi_load(filename, &textureSize, &textureSize, &numComponents, 0);

	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureSize, textureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	// Completed generating the texture, free the image memory.
	stbi_image_free(image);

	return texId;
}

void Shader::Update(glm::mat4 MVP, glm::mat4 Normal, int i)
{
	glUniformMatrix4fv(m_uniforms[0], 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(m_uniforms[1], 1, GL_FALSE, &Normal[0][0]);
	glUniform3f(m_uniforms[2], 0.0f, 0.0f, 1.0f);
	glUniform3f(m_uniforms[3], 1.0f, 1.0f, 1.0f);

	glUniformMatrix4fv(m_uniforms[4], 1, GL_FALSE, &MVP[0][0]);

	// Convert "i", the integer mesh ID, into an RGB color
	int r = (i & 0x000000FF) >> 0;
	int g = (i & 0x0000FF00) >> 8;
	int b = (i & 0x00FF0000) >> 16;

	// OpenGL expects colors to be in [0,1], so divide by 255.
	glUniform4f(m_uniforms[4], r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
	/*Texture();*/
}

std::string Shader::LoadShader(const std::string& fileName)
{
    std::ifstream file;
    file.open((fileName).c_str());

    std::string output;
    std::string line;

    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
			output.append(line + "\n");
        }
    }
    else
    {
		std::cerr << "Unable to load shader: " << fileName << std::endl;
    }

    return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if(isProgram)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);

    if(success == GL_FALSE)
    {
        if(isProgram)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);

        std::cerr << errorMessage << ": '" << error << "'" << std::endl;
    }
}

GLuint Shader::CreateShader(const std::string& text, unsigned int type)
{
    GLuint shader = glCreateShader(type);

    if(shader == 0)
		std::cerr << "Error compiling shader type " << type << std::endl;

    const GLchar* p[1];
    p[0] = text.c_str();
    GLint lengths[1];
    lengths[0] = text.length();

    glShaderSource(shader, 1, p, lengths);
    glCompileShader(shader);

    CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

    return shader;
}


