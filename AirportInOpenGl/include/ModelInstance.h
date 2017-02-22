#pragma once
#include "Model.h"
#include "MathFunc.h"
class ModelInstance
{
public:

	ModelInstance(Model* model, vec3 position, vec3 roatation, vec3 scale);
	void draw(GLuint& shaderProgram);
	void setPosition(vec3 position);

private:
	Model* m_model;
	vec3 m_position;
	vec3 m_rotation;
	vec3 m_scale;
	mat4 m_modelMatrix;
};

