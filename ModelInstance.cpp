#include "stdafx.h"
#include "ModelInstance.h"
#include "Win32OpenGl.h"


ModelInstance::ModelInstance(Model* model, vec3 position, vec3 roatation, vec3 scale)
	:m_model{ model },
	m_position{position},
	m_rotation{roatation},
	m_scale{scale}
{
	
}

void ModelInstance::draw(GLuint& shaderProgram)
{
	m_modelMatrix = identity_mat4();
	m_modelMatrix = scale(m_modelMatrix, m_scale);
	m_modelMatrix = translate(m_modelMatrix, m_position);
	m_modelMatrix = rotate_x_deg(m_modelMatrix, m_rotation.v[0]);
	m_modelMatrix = rotate_y_deg(m_modelMatrix, m_rotation.v[1]);
	m_modelMatrix = rotate_z_deg(m_modelMatrix, m_rotation.v[2]);
	
	Win32OpenGL::SendUniformMatrixToShader(shaderProgram, m_modelMatrix.m, "model_matrix");
	m_model->draw();
}

void ModelInstance::setPosition(vec3 position)
{
	m_position = position;
}
