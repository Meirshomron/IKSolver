#include <SceneData.h>

SceneData::SceneData(vec3 m_pos, vec3 m_forward, vec3 m_up) :pos(vec3(m_pos)), forward(vec3(m_forward)), up(vec3(m_up))
{
	mainMat = mat4(1);
	MVP = mat4(1);
}

SceneData::~SceneData()
{
}

void SceneData::setPos(vec3 m_pos)
{
	pos = vec3(m_pos);
}

void SceneData::setForward(vec3 m_forward)
{
	forward = vec3(m_forward);
}

void SceneData::setUp(vec3 m_up)
{
	up = vec3(m_up);
}

vec3 SceneData::getPos()
{
	return pos;
}

vec3 SceneData::getForward()
{
	return forward;
}

vec3 SceneData::getUp()
{
	return up;
}

void SceneData::setPerspectiveProjection(float y, float aspectRatio, float zNear, float zFar)
{
	ProjectionMat = perspective(y, aspectRatio, zNear, zFar);
}

void SceneData::setProjection(mat4 proj)
{
	ProjectionMat = mat4(proj);
}

mat4 SceneData::getProjection()
{
	return ProjectionMat;
}

mat4 SceneData::TransformMainMat(mat4 transformation)
{
	mainMat = transformation*mainMat;
	return mainMat;
}

void SceneData::setMainMat(mat4 toSet)
{
	mainMat = mat4(toSet);
}

void SceneData::muliplyMVP()
{
	MVP = ProjectionMat*mainMat;
}

mat4 SceneData::getMVP()
{
	return MVP;
}

mat4 SceneData::getMainMat()
{
	return mainMat;
}