#pragma once

#include "Config.h"
#include <iostream>
#include "glm\glm.hpp"
#include <Cube.h>
#include <SceneData.h>
#include "shader.h"
#include "display.h"
#include <GLFW/glfw3.h>

// Math parameters.
#define M_PI 3.14159265358979323846
#define X_AXIS vec3(1.0f, 0, 0)
#define Y_AXIS vec3(0, 1.0f, 0)
#define Z_AXIS vec3(0, 0, 1.0f)

// IK Solver parameters, the chain starts from index 0 till before last cube that is the target.
static const int NUM_OF_CUBES = 7;
static const int NUM_OF_LINKS = NUM_OF_CUBES - 1;
static const int TARGET_CUBE_INDEX = NUM_OF_CUBES - 1;
static const int BASE_LINK_INDEX = 0;

static const vec3 LINK_SIZE = vec3(2.0f, 2.0f, 4.0f);
static const vec3 TARGET_SIZE = vec3(2.0f, 2.0f, 2.0f);
static const vec3 TARGET_START_POSITION = vec3(5.0f, 0.0f, 0.0f);

//Scene parameters
static const float fovy = 60.0;
static const float zNear = 0.1;
static const float zFar = 100.0;

using namespace glm;
using namespace Config;

class IKSolver
{
	public:
		IKSolver();
		void handleArrowRotation(int axis, int dir);
		void handleLeftMouseDragging(float curX, float prevX, float curY, float prevY);
		void handleRightMouseDragging(float transX, float transY);
		void handleMouseCallback(float xpos, float ypos);
		void handleScrollCallback(float yoffset);
		void spacePressed();
		void draw();

		~IKSolver();
	private:
		void runCCDSolverAlgorithm();
		void drawLinksAxisSystem();

		mat4 m_cubeTranslations[NUM_OF_CUBES];
		mat4 m_cubeTransformations[NUM_OF_CUBES];
		mat4 m_cubeRotations[NUM_OF_LINKS];

		mat4 m_rotateX[NUM_OF_LINKS];
		mat4 m_rotateZ[NUM_OF_LINKS];
		mat4 m_rotateZ2[NUM_OF_LINKS];

		mat4 m_linkBottomPoint, m_linkTopPoint;

		Cube* m_link;
		Cube* m_target;
		Shader* m_shader;
		Shader* m_pickingShader;
		SceneData* m_scene;

		unsigned int m_chainTextureId;
		unsigned int m_targetTextureId;

		bool m_isStopped;
		bool m_isTargetOutOfReach;
		vec4 m_lastReachedTargetPoint;
		int m_angleSizeFactor;
		int m_pressedIndex;
};

