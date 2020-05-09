#include "Cube.h"

Cube::Cube(vec3 position, vec3 size, vec3 singleColor)
{
	initVertices(position, size, singleColor);
	initIndices();
}

/**
* Init the indices of the vertices, every cube face is defined by 2 triangles = 4 vertices = 6 indices.
*/
void Cube::initIndices()
{
	m_indices[0] = 0;
	m_indices[1] = 1;
	m_indices[2] = 2;

	m_indices[3] = 0;
	m_indices[4] = 2;
	m_indices[5] = 3;

	m_indices[6] = 6;
	m_indices[7] = 5;
	m_indices[8] = 4;

	m_indices[9] = 7;
	m_indices[10] = 6;
	m_indices[11] = 4;

	m_indices[12] = 10;
	m_indices[13] = 9;
	m_indices[14] = 8;

	m_indices[15] = 11;
	m_indices[16] = 10;
	m_indices[17] = 8;

	m_indices[18] = 12;
	m_indices[19] = 13;
	m_indices[20] = 14;

	m_indices[21] = 12;
	m_indices[22] = 14;
	m_indices[23] = 15;

	m_indices[24] = 16;
	m_indices[25] = 17;
	m_indices[26] = 18;

	m_indices[27] = 16;
	m_indices[28] = 18;
	m_indices[29] = 19;

	m_indices[30] = 22;
	m_indices[31] = 21;
	m_indices[32] = 20;

	m_indices[33] = 23;
	m_indices[34] = 22;
	m_indices[35] = 20;
}

/**
* Init the vertices of this cube, their postions, textures, normals and colors.
* Every face of the cube is represented by 4 vertices with the same color and normal.
*
* @param x Starting x position.
* @param y Starting y position.
* @param z Starting z position.
* @param size Cube size.
*/
void Cube::initVertices(vec3 position, vec3 size, vec3 singleColor)
{
	// Every Vertex struct includes 3 vec3 and 1 vec2.
	int vertexSize = sizeof(void *) * 3 * sizeof(vec3) + sizeof(vec2);
	m_vertices = (Vertex*)malloc(24 * vertexSize);

	float x = position.x;
	float y = position.y;
	float z = position.z;

	float halfSizeX = size.x / 2.0f;
	float halfSizeY = size.y / 2.0f;
	float halfSizeZ = size.z / 2.0f;

	vec3 currentColor;

	// Yellow face.
	currentColor = (singleColor.x == -1.0f) ? vec3(1, 1, 0) : singleColor;
	m_vertices[0] = Vertex(vec3(-halfSizeX + x, -halfSizeY + y, -halfSizeZ + z), vec2(1, 0), vec3(0, 0, -1), currentColor);
	m_vertices[1] = Vertex(vec3(-halfSizeX + x, halfSizeY + y, -halfSizeZ + z), vec2(0, 0), vec3(0, 0, -1), currentColor);
	m_vertices[2] = Vertex(vec3(halfSizeX + x, halfSizeY + y, -halfSizeZ + z), vec2(0, 1), vec3(0, 0, -1), currentColor);
	m_vertices[3] = Vertex(vec3(halfSizeX + x, -halfSizeY + y, -halfSizeZ + z), vec2(1, 1), vec3(0, 0, -1), currentColor);

	// White face.
	currentColor = (singleColor.x == -1.0f) ? vec3(1, 1, 1) : singleColor;
	m_vertices[4] = Vertex(vec3(-halfSizeX + x, -halfSizeY + y, halfSizeZ + z), vec2(1, 0), vec3(0, 0, 1), currentColor);
	m_vertices[5] = Vertex(vec3(-halfSizeX + x, halfSizeY + y, halfSizeZ + z), vec2(0, 0), vec3(0, 0, 1), currentColor);
	m_vertices[6] = Vertex(vec3(halfSizeX + x, halfSizeY + y, halfSizeZ + z), vec2(0, 1), vec3(0, 0, 1), currentColor);
	m_vertices[7] = Vertex(vec3(halfSizeX + x, -halfSizeY + y, halfSizeZ + z), vec2(1, 1), vec3(0, 0, 1), currentColor);

	// Orange face.
	currentColor = (singleColor.x == -1.0f) ? vec3(1, 0.5, 0) : singleColor;
	m_vertices[8] = Vertex(vec3(-halfSizeX + x, -halfSizeY + y, -halfSizeZ + z), vec2(0, 1), vec3(0, -1, 0), currentColor);
	m_vertices[9] = Vertex(vec3(-halfSizeX + x, -halfSizeY + y, halfSizeZ + z), vec2(1, 1), vec3(0, -1, 0), currentColor);
	m_vertices[10] = Vertex(vec3(halfSizeX + x, -halfSizeY + y, halfSizeZ + z), vec2(1, 0), vec3(0, -1, 0), currentColor);
	m_vertices[11] = Vertex(vec3(halfSizeX + x, -halfSizeY + y, -halfSizeZ + z), vec2(0, 0), vec3(0, -1, 0), currentColor);

	// Red face.
	currentColor = (singleColor.x == -1.0f) ? vec3(1, 0, 0) : singleColor;
	m_vertices[12] = Vertex(vec3(-halfSizeX + x, halfSizeY + y, -halfSizeZ + z), vec2(0, 1), vec3(0, 1, 0), currentColor);
	m_vertices[13] = Vertex(vec3(-halfSizeX + x, halfSizeY + y, halfSizeZ + z), vec2(1, 1), vec3(0, 1, 0), currentColor);
	m_vertices[14] = Vertex(vec3(halfSizeX + x, halfSizeY + y, halfSizeZ + z), vec2(1, 0), vec3(0, 1, 0), currentColor);
	m_vertices[15] = Vertex(vec3(halfSizeX + x, halfSizeY + y, -halfSizeZ + z), vec2(0, 0), vec3(0, 1, 0), currentColor);

	// Blue face.
	currentColor = (singleColor.x == -1.0f) ? vec3(0, 0, 1) : singleColor;
	m_vertices[16] = Vertex(vec3(-halfSizeX + x, -halfSizeY + y, -halfSizeZ + z), vec2(1, 1), vec3(-1, 0, 0), currentColor);
	m_vertices[17] = Vertex(vec3(-halfSizeX + x, -halfSizeY + y, halfSizeZ + z), vec2(1, 0), vec3(-1, 0, 0), currentColor);
	m_vertices[18] = Vertex(vec3(-halfSizeX + x, halfSizeY + y, halfSizeZ + z), vec2(0, 0), vec3(-1, 0, 0), currentColor);
	m_vertices[19] = Vertex(vec3(-halfSizeX + x, halfSizeY + y, -halfSizeZ + z), vec2(0, 1), vec3(-1, 0, 0), currentColor);

	// Green face.
	currentColor = (singleColor.x == -1.0f) ? vec3(0, 1, 0) : singleColor;
	m_vertices[20] = Vertex(vec3(halfSizeX + x, -halfSizeY + y, -halfSizeZ + z), vec2(1, 1), vec3(1, 0, 0), currentColor);
	m_vertices[21] = Vertex(vec3(halfSizeX + x, -halfSizeY + y, halfSizeZ + z), vec2(1, 0), vec3(1, 0, 0), currentColor);
	m_vertices[22] = Vertex(vec3(halfSizeX + x, halfSizeY + y, halfSizeZ + z), vec2(0, 0), vec3(1, 0, 0), currentColor);
	m_vertices[23] = Vertex(vec3(halfSizeX + x, halfSizeY + y, -halfSizeZ + z), vec2(0, 1), vec3(1, 0, 0), currentColor);
}

void Cube::draw()
{
	Mesh mesh(m_vertices, 24, m_indices, sizeof(m_indices) / sizeof(m_indices[0]));
	mesh.Draw();
}

Cube::~Cube()
{
	delete m_vertices;
}