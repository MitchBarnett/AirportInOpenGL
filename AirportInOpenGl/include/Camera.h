#pragma once

#include "MathFunc.h"
#include "win32OpenGl.h"

/*!
A viewpoint from which the scene will be rendered.


*/
class Camera
{
public:
	Camera();
	~Camera();

	float m_lookSensitivity = 1.0f;
	float m_moveSpeed = 1.1f;

	const float startX = 0.0f;
	const float startY = 1.5f;
	const float startZ = 5.0f;

	vec3 m_position{ startX, startY, startZ };


	float m_yaw{ 0 };
	float m_roll{ 0 };
	float m_pitch{ 0 };
	
	vec3 m_up{ 0,1,0 };
	vec3 m_forward{ 0, 0, 0 };
	//float m_directionX{ 0 };
	//float m_directionY{ 0 };
	//float m_directionZ{ 0 };

	mat4 m_viewMatrix;

	void computeViewMatrixUsingLookAt();
	void computeDirectionVector();
	void update();
	void updateCameraPosition(float distanceMovedForward, float distanceMovedRight);
	void setViewMatrix(GLuint program);
	void handleInput(int mouseMoveX, int mouseMoveY);

};

