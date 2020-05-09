#pragma once

#include "mesh.h"

using namespace glm;

class Cube
{
public:
	Cube(vec3 position, vec3 size, vec3 singleColor = vec3(-1.0f));
	void draw();
	~Cube();

private:
	void initVertices(vec3 position, vec3 size, vec3 singleColor = vec3(-1.0f));
	void initIndices();

	Vertex* m_vertices;
	unsigned int m_indices[36];
};