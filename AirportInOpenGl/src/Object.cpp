#include "stdafx.h"
#include "Object.h"
#include "Win32OpenGl.h"


Object::Object(Model* model, vec3 position, vec3 roatation, vec3 scale)
	:m_model{ model },
	m_position{position},
	m_rotation{roatation},
	m_scale{scale}
{
	
}

void Object::draw(GLuint& shaderProgram)
{
	m_modelMatrix = identity_mat4();
	m_modelMatrix = scale(m_modelMatrix, m_scale);
	m_modelMatrix = rotate_x_deg(m_modelMatrix, m_rotation.v[0]);
	m_modelMatrix = rotate_y_deg(m_modelMatrix, m_rotation.v[1]);
	m_modelMatrix = rotate_z_deg(m_modelMatrix, m_rotation.v[2]);
	m_modelMatrix = translate(m_modelMatrix, m_position);
	
	
	Win32OpenGL::SendUniformMatrixToShader(shaderProgram, m_modelMatrix.m, "model_matrix");
	m_model->draw();
}

void Object::setPosition(vec3 position)
{
	m_position = position;
}
