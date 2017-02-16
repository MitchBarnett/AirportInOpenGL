#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::computeViewMatrixUsingLookAt()
{
	computeDirectionVector();
	//	float eyeX = m_x;
	//	float eyeY = m_y;
	//	float eyeZ = m_z;

	//	float centerX = m_x + m_directionX;
	//	float centerY = m_y + m_directionY;
	//	float centerZ = m_z + m_directionZ;

	float upX = 0;
	float upY = 1;
	float upZ = 0;

	//vec3 forward = vec3(centerX - eyeX, centerY - eyeY, centerZ - eyeZ); - simplified now...
	// we already store centreX - eyeX as m_directionX
	vec3 forward = m_forward;
	vec3 up = vec3(upX, upY, upZ);
	vec3 forwardNormalised = normalise(forward);		//normalize(forward);
	vec3 side = vec3(forwardNormalised);				// copy cross(forward, up, side);
	side = cross(side, up);
	vec3 sideNormalised = normalise(side);				//normalize(side);
	up = vec3(sideNormalised);
	up = cross(up, forwardNormalised);					// no need to normalise as generated from 2 normalised vectors of unit length

														// now we build the matrix... (column major)
														// this is the transpose for the V matrix as  Vinverse == VT for rotation matrices
														// row 0
	m_viewMatrix.m[0] = sideNormalised.v[0];
	m_viewMatrix.m[4] = sideNormalised.v[1];
	m_viewMatrix.m[8] = sideNormalised.v[2];
	m_viewMatrix.m[12] = 0;

	// row 1
	m_viewMatrix.m[1] = up.v[0];
	m_viewMatrix.m[5] = up.v[1];
	m_viewMatrix.m[9] = up.v[2];
	m_viewMatrix.m[13] = 0;

	// row 2
	m_viewMatrix.m[2] = -(forwardNormalised.v[0]);
	m_viewMatrix.m[6] = -(forwardNormalised.v[1]);
	m_viewMatrix.m[10] = -(forwardNormalised.v[2]);
	m_viewMatrix.m[14] = 0;

	// row 3
	m_viewMatrix.m[3] = 0;
	m_viewMatrix.m[7] = 0;
	m_viewMatrix.m[11] = 0;
	m_viewMatrix.m[15] = 1;

	// now apply translation to camera position

	mat4 cameraTranslation = translate(identity_mat4(), m_position * -1);

	m_viewMatrix = m_viewMatrix * cameraTranslation;  // pre or post multiply - - definitely POST!!!
}

void Camera::computeDirectionVector()
{
	float angleYZRadians = 3.14156f * m_angleYZ / 180.0f;

	m_forward.v[1] = sin(angleYZRadians);

	float m_other = cos(angleYZRadians);

	float angleXZRadians = 3.14156f * m_angleXZ / 180.0f;
	m_forward.v[0] = m_other * sin(angleXZRadians);
	m_forward.v[2] = m_other * -cos(angleXZRadians);

}

void Camera::updateCameraPosition(float distanceMovedForward, float distanceMovedRight)
{
	// later might use matrices to calculate this....
	// for now use angle in xz and stay on ground....
	// if moved update position
	vec3 right = cross(m_forward, m_up);
	m_position += m_forward * distanceMovedForward;
	m_position += right * distanceMovedRight;
	/*m_position.v[0] += distanceMoved * m_directionX;
	m_position.v[1] += distanceMoved * m_directionY;
	m_position.v[2] += distanceMoved * m_directionZ;*/

}

void Camera::setViewMatrix(GLuint program)
{
	Win32OpenGL::SendUniformMatrixToShader(program, m_viewMatrix.m, "view_matrix");
}

void Camera::handleInput(unsigned char keyCode)
{
	vec3 movementVector{0,0,0};
	float distanceMovedForward = 0.0f;
	float distanceMovedRight = 0.0f;

	float angleStep = 2.5f;

	if (keyCode == VK_LEFT)
	{
		m_angleXZ -= angleStep;
	}
	else if (keyCode == VK_RIGHT)
	{
		m_angleXZ += angleStep;
	}
	else if (keyCode == VK_UP)
	{
		m_angleYZ += angleStep;
	}
	else if (keyCode == VK_DOWN)
	{
		m_angleYZ -= angleStep;
	}
	else if (keyCode == 'W')
	{
		distanceMovedForward = 0.1f;
	}
	else if (keyCode == 'S')
	{
		distanceMovedForward = -0.1f;
	}
	else if (keyCode == 'A')
	{
		distanceMovedRight = -0.1;
	}
	else if (keyCode == 'D')
	{
		distanceMovedRight = +0.1;
	}
	else if (keyCode == VK_SHIFT)
	{
		m_position.v[1] += 0.1;
	}
	else if (keyCode == VK_CONTROL)
	{
		m_position.v[1] -= 0.1;
	}

	// MORE KEY RESPONSES HERE
	// move camera and update view matrix

	computeDirectionVector();

	updateCameraPosition(distanceMovedForward, distanceMovedRight);
	computeViewMatrixUsingLookAt();

}

void Camera::handleInput(int mouseMoveX, int mouseMoveY)
{
	m_angleXZ += mouseMoveX * (lookSensitivity / 10);
	m_angleYZ += mouseMoveY * (lookSensitivity / 10);
	computeDirectionVector();
	updateCameraPosition(0, 0);
	computeViewMatrixUsingLookAt();
}

