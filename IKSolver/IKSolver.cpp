#include "IKSolver.h"

IKSolver::IKSolver()
{
	m_pressedIndex = -1;

	m_shader = new Shader("./res/shaders/basicShader");
	m_pickingShader = new Shader("./res/shaders/pickingShader");

	// Initialize the scene parameters.
	m_scene = new SceneData(vec3(0, 5, -45), Z_AXIS, Y_AXIS);
	m_scene->setPerspectiveProjection(fovy, ASPECT_RATIO, zNear, zFar);
	m_scene->setProjection(m_scene->getProjection() * lookAt(m_scene->getPos(), m_scene->getPos() + m_scene->getForward(), m_scene->getUp()));
	m_scene->muliplyMVP();

	// Initialize the rest of the member parameters.
	m_link = new Cube(vec3(0), LINK_SIZE);
	m_target = new Cube(vec3(0), TARGET_SIZE, vec3(1, 0.5, 1));
	m_linkBottomPoint = translate(vec3(0, 0, -LINK_SIZE.z / 2));
	m_linkTopPoint = translate(vec3(0, 0, LINK_SIZE.z / 2));

	m_lastReachedTargetPoint = vec4(INFINITY);
	m_isTargetOutOfReach = false;
	m_angleSizeFactor = 25;
	m_isStopped = true;

	// Rotate the scene's projection 90 degrees around the x axis.
	m_scene->setProjection(rotate(m_scene->getProjection(), -90.0f, X_AXIS));

	// Initialize cube transformation matrices.
	for (int i = 0; i < NUM_OF_CUBES; ++i) 
	{
		// Rotations not enabled on the target, only on the chain.
		if (i < NUM_OF_LINKS)
		{
			m_cubeRotations[i] = mat4(1.0);

			// ZXZ Euler Angles as three successive rotations around z, x, and z axes.
			// ZXZ Euler Angles are also known as proper Euler Angles.
			m_rotateX[i] = mat4(1.0);
			m_rotateZ[i] = mat4(1.0);
			m_rotateZ2[i] = mat4(1.0);
		}
		m_cubeTranslations[i] = mat4(1.0);
		m_cubeTransformations[i] = mat4(1.0);

		// Set the starting position of every link.
		if (i > BASE_LINK_INDEX && i < NUM_OF_LINKS)
		{
			m_cubeTranslations[i] = translate(m_cubeTranslations[i], vec3(0.0f, 0.0f, LINK_SIZE.z * 1.0f));
		}
		else if (i == TARGET_CUBE_INDEX)
		{
			m_cubeTranslations[i] = translate(m_cubeTranslations[i], TARGET_START_POSITION);
		}
	}

	// Initialize 2 textures, 1 for the chain and 1 for the target.
	// Before drawing a cube, bind its matching texture id.
	m_chainTextureId = m_shader->Texture("./res/textures/box0.bmp");
	m_targetTextureId = m_shader->Texture("./res/textures/bricks.jpg");
}

/*
* drawLinksAxisSystem
* 
* @tbrief Draws the axis of every box in the chain.
*/
void IKSolver::drawLinksAxisSystem()
{	
	// X axis line at the bottom of the link.
	glBegin(GL_LINES);
	glVertex3f(-10.0f, 0.0f, -LINK_SIZE.z / 2);
	glVertex3f(10.0f, 0.0f, -LINK_SIZE.z / 2);

	// Y Axis line at the bottom of the link.
	glBegin(GL_LINES);
	glVertex3f(0.0f, 10.0f, -LINK_SIZE.z / 2);
	glVertex3f(0.0f, -10.0f, -LINK_SIZE.z / 2);

	// Z axis line.
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 10.0f);
	glVertex3f(0.0f, 0.0f, -10.0f);
	glEnd();
}

void IKSolver::spacePressed()
{
	m_isStopped = !m_isStopped;
	if (!m_isStopped)
	{
		runCCDSolverAlgorithm();
	}
}

/*
* runCCDSolverAlgorithm
*
* @tbrief IK solver by the CCD algorithm.
*/
void IKSolver::runCCDSolverAlgorithm()
{		
	float threshold = 0.1f;
	float chainMaxLength = LINK_SIZE.z * NUM_OF_LINKS;

	// Top of the last part in the chain, every link's position represents it's middle so we raise it have its length to get to it's top point.
	vec4 chainTopPoint = m_cubeTransformations[NUM_OF_LINKS - 1] * m_linkTopPoint * vec4(1, 1, 0, 1);

	// Bottom of the base part in the chain.
	vec4 chainBottomPoint = m_cubeTransformations[0] * m_linkBottomPoint * vec4(1);

	// Destination = target tranformations + offset on the target.
	vec4 targetPoint = m_cubeTransformations[TARGET_CUBE_INDEX] * translate(vec3(-2, 0, -1)) * vec4(1);
	
	// Target too far.
	if (distance(targetPoint, chainBottomPoint) > chainMaxLength)
	{
		// Only print "cannot reach" when changing status from can reach to can't reach.
		if (!m_isTargetOutOfReach)
		{
			std::cout << "cannot reach" << std::endl;
			m_isTargetOutOfReach = true;
		}
	}
	// Target reached.
	else if (distance(targetPoint, chainTopPoint) <= threshold)
	{
		bool targetPointChanged = (distance(m_lastReachedTargetPoint, targetPoint) > threshold);
		if (targetPointChanged)
		{
			m_lastReachedTargetPoint = targetPoint;
			std::cout << distance(targetPoint, chainTopPoint) << std::endl;
		}
	}
	// Move closer to the target.
	else
	{
		// std::cout << "targetPoint = (" << targetPoint.x << ", " << targetPoint.y << ", " << targetPoint.z << ", " << targetPoint.w << ")" << std::endl;
		// std::cout << "chainTopPoint = (" << chainTopPoint.x << ", " << chainTopPoint.y << ", " << chainTopPoint.z << ", " << chainTopPoint.w << ")" << std::endl;

		m_isTargetOutOfReach = false;

		// For every part in the chain rotate it a bit according to the algorithm.
		for (int i = (NUM_OF_LINKS - 1); i >= BASE_LINK_INDEX; i--) 
		{
			// r = link root, e = chain end, d = desired endpoint, re = vector from r to e, rd = vector from r to d.
			vec4 r = m_cubeTransformations[i] * m_linkBottomPoint * vec4(1);
			vec4 re = normalize(chainTopPoint - r);
			vec4 rd = normalize(targetPoint - r);

			// rotate the current link around the (re X rd) posture vector  by the angle between re and rd and the angle size factor.
			m_cubeRotations[i] = m_linkBottomPoint * rotate(degrees(acos(clamp(dot(re, rd), -1.0f, 1.0f))) / m_angleSizeFactor, normalize(cross((vec3)re, (vec3)rd))) * m_linkTopPoint * m_cubeRotations[i];

			chainTopPoint = m_cubeTransformations[NUM_OF_LINKS - 1] * m_linkTopPoint * vec4(1, 1, 0, 1);
		}
	}
}

/*
* handleArrowRotation
*
* @tbrief Rotations according to arrows. If a link in the chain was selected then rotate it, otherwise rotate the scene. 
* @tparam axis, 0 In case of left / right to rotate around the z axis, 1 in case of up / down to rotate around th x axis.
* @tparam dir, 1 For clockwise or -1 for counter-clockwise direction.
*/
void IKSolver::handleArrowRotation(int axis, int dir)
{		
	float rotationSpeed = 3;

	// Pressed a link in the chain,  rotate that link and that will rotateall the links above it.
	if (m_pressedIndex >= BASE_LINK_INDEX && m_pressedIndex < NUM_OF_LINKS)
	{
		// Pressed top / bottom arrow, rotate aroud the x axis.
		if (axis)
		{
			m_rotateX[m_pressedIndex] = m_linkBottomPoint * rotate(dir * rotationSpeed, X_AXIS) * m_linkTopPoint * m_rotateX[m_pressedIndex];
		}
		// Pressed left / right arrow, rotate aroud the z axis.
		else
		{
			m_rotateZ[m_pressedIndex] = m_linkBottomPoint * rotate(dir * rotationSpeed, Z_AXIS) * m_linkTopPoint * m_rotateZ[m_pressedIndex];
			m_rotateZ2[m_pressedIndex] = m_linkBottomPoint * rotate(dir * rotationSpeed, -Z_AXIS) * m_linkTopPoint * m_rotateZ2[m_pressedIndex];
		}
	}
	// Not pressed a link in the chain, rotate the scene.
	else if (m_pressedIndex == -1)
	{
		if (axis)
		{
			m_scene->setProjection(rotate(m_scene->getProjection(), dir * rotationSpeed, X_AXIS));
		}
		else
		{
			m_scene->setProjection(rotate(m_scene->getProjection(), dir * rotationSpeed, Z_AXIS));
		}
	}
}

/*
* handleLeftMouseDragging
*
* @tbrief Rotations according to mouse draggings. If a link in the chain was selected then rotate it, otherwise rotate the scene.
* @tparam curX, Mouse current x position.
* @tparam prevX Mouse previous x position.
* @tparam curY Mouse current y position.
* @tparam prevY Mouse previous y position.
*/
void IKSolver::handleLeftMouseDragging(float curX, float prevX, float curY, float prevY)
{
	float angle = 0.5;

	if (m_pressedIndex >= BASE_LINK_INDEX && m_pressedIndex < NUM_OF_LINKS)
	{
		m_rotateX[m_pressedIndex] = m_linkBottomPoint * rotate((float)(curY - prevY) * angle, X_AXIS) * m_linkTopPoint * m_rotateX[m_pressedIndex];
		m_rotateZ[m_pressedIndex] = m_linkBottomPoint * rotate((float)(curX - prevX) * angle, Z_AXIS) * m_linkTopPoint * m_rotateZ[m_pressedIndex];
		m_rotateZ2[m_pressedIndex] = m_linkBottomPoint * rotate((float)(curX - prevX) * angle, -Z_AXIS) * m_linkTopPoint * m_rotateZ2[m_pressedIndex];
	}
	else if (m_pressedIndex == -1)
	{
		m_scene->setProjection(rotate(m_scene->getProjection(), (float)(curY - prevY) * angle, X_AXIS));
		m_scene->setProjection(rotate(m_scene->getProjection(), (float)(curX - prevX) * angle, Z_AXIS));
	}
}

/*
* handleRightMouseDragging
*
* @tbrief drag selected (chain / target / non selected - scene) under the mouse's position.
* @tparam transX, Mouse current x position in camera coordinates.
* @tparam transY Mouse previous y position in camera coordinates.
*/
void IKSolver::handleRightMouseDragging(float transX, float transY)
{
	if (m_pressedIndex >= BASE_LINK_INDEX && m_pressedIndex < NUM_OF_LINKS)
	{
		m_cubeTranslations[BASE_LINK_INDEX] = translate(vec3(transX, 0, transY)) * m_cubeTranslations[BASE_LINK_INDEX];
	}
	else if (m_pressedIndex == TARGET_CUBE_INDEX)
	{
		m_cubeTranslations[TARGET_CUBE_INDEX] = translate(vec3(transX, 0, transY)) * m_cubeTranslations[TARGET_CUBE_INDEX];
	}
	else
	{
		m_cubeTranslations[BASE_LINK_INDEX] = translate(vec3(transX, 0, transY)) * m_cubeTranslations[BASE_LINK_INDEX];
		m_cubeTranslations[TARGET_CUBE_INDEX] = translate(vec3(transX, 0, transY)) * m_cubeTranslations[TARGET_CUBE_INDEX];
	}
}

/*
* handleScrollCallback
* 
* @tbrief Mouse scroller zooms in and out.
* @tparam offsetY The amount of scrolling on the y axis.
*/
void IKSolver::handleScrollCallback(float offsetY)
{
	// If offsetY >= 0 we're scrolling up so scroll backwards, otherwise scroll forwards.  
	int direction = offsetY >= 0 ? -1 : 1;

	if (m_pressedIndex >= BASE_LINK_INDEX && m_pressedIndex < NUM_OF_LINKS)
	{
		m_cubeTranslations[BASE_LINK_INDEX] = m_cubeTranslations[BASE_LINK_INDEX] * translate(vec3(0, direction, 0));
	}
	else if (m_pressedIndex == TARGET_CUBE_INDEX)
	{
		m_cubeTranslations[TARGET_CUBE_INDEX] = m_cubeTranslations[TARGET_CUBE_INDEX] * translate(vec3(0, direction, 0));
	}
}

/*
* handleMouseCallback
*
* @tbrief Mouse press, calculate the pressed index with a picking shader, scene index = -1, chain index = 0 .. NUM_OF_LINKS, target = TARGET_CUBE_INDEX.
* @tparam xpos Mouse pressed x position.
* @tparam ypos Mouse pressed y position.
*/
void IKSolver::handleMouseCallback(float xpos, float ypos)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pickingShader->Bind();

	for (int i = 0; i < NUM_OF_CUBES; i++)
	{
		m_scene->setMainMat(m_cubeTransformations[i]);
		m_scene->muliplyMVP();

		// The index we passed is translated to an rgb color in the shader.
		m_pickingShader->Update(mat4(m_scene->getMVP()), mat4(m_scene->getMainMat()), i);

		if (i < NUM_OF_LINKS)
		{
			m_link->draw();
		}
		else if (i == TARGET_CUBE_INDEX)
		{
			m_target->draw();
		}
	}	
	unsigned char data[4];
	glReadPixels(xpos, DISPLAY_HEIGHT - ypos - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	// Convert back from rgb color to the index we passed.
	int pickedID = data[0] + data[1] * 256 + data[2] * 256 * 256;

	// Full white, must be the background.
	if (pickedID == 0x00ffffff)
	{ 
		m_pressedIndex = -1;
	}
	else 
	{
		m_pressedIndex = pickedID;
	}
}

/*
* draw
*
* @tbrief Called every game loop's draw iteration, render the scene to the window.
*/
void IKSolver::draw()
{
	// Iterate all the chain links and the target.
	for (int i = 0; i < NUM_OF_CUBES; i++)
	{
		if (i == BASE_LINK_INDEX)
		{
			m_cubeTransformations[i] = m_cubeTranslations[i] * m_cubeRotations[i] * m_rotateZ2[i] * m_rotateX[i] * m_rotateZ[i];
			// Bind the m_chainTextureId texture of the chain, we only do this for the base link because the shader only changes after the last link, for the target.
			m_shader->bindTexture(m_chainTextureId);
		}
		else if (i < NUM_OF_LINKS)
		{	
			// Calculate transformations according to the previous link.
			m_cubeTransformations[i] = m_cubeTransformations[i - 1] * m_cubeTranslations[i] * m_cubeRotations[i] * m_rotateZ2[i] * m_rotateX[i] * m_rotateZ[i];
		}
		else if (i == TARGET_CUBE_INDEX)
		{			
			// Set the target's tranformtions and bind it's shader.
			m_cubeTransformations[i] = m_cubeTranslations[i];
			m_shader->bindTexture(m_targetTextureId);
		}
		 
		// Set the scene's MVP according to the current object's transformations.
		m_scene->setMainMat(m_cubeTransformations[i]);
		m_scene->muliplyMVP();
		m_shader->Bind();
		m_shader->Update(m_scene->getMVP(), m_scene->getMainMat());

		// Draw the current object.
		if (i < NUM_OF_LINKS)
		{
			m_link->draw();
			drawLinksAxisSystem();
		}
		else if (i == TARGET_CUBE_INDEX)
		{
			m_target->draw();
		}
	}
	if (!m_isStopped)
	{
		runCCDSolverAlgorithm();
	}
}

IKSolver::~IKSolver()
{
	// Delete texture objects.
	glDeleteTextures(1, &m_chainTextureId);
	glDeleteTextures(1, &m_targetTextureId);
}
