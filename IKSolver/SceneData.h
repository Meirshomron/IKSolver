#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

class SceneData
{
public:
	SceneData(vec3 m_pos, vec3 m_forward, vec3 m_up);
	~SceneData();
	void setPos(vec3 m_pos);
	void setForward(vec3 m_forward);
	void setUp(vec3 m_up);
	vec3 getPos();
	vec3 getForward();
	vec3 getUp();
	void setPerspectiveProjection(float y, float aspectRatio, float zNear, float zFar);
	void setProjection(mat4 proj);
	mat4 getProjection();
	mat4 TransformMainMat(mat4 transformation);
	void setMainMat(mat4 toSet);
	void muliplyMVP();
	mat4 getMVP();
	mat4 getMainMat();

private:
	vec3 pos;
	vec3 forward;
	vec3 up;
	mat4 ProjectionMat;
	mat4 mainMat;
	mat4 MVP;
};