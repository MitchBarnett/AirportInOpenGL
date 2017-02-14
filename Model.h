#pragma once
#include "Win32OpenGL.h"
#include "resource.h"
#include "ModelLoader.h"
#include "TextureLoader.h"
#include "Log.h"

class Model
{
public:
	Model(string modelLoacation, string textrueLocation);
	Model(string modelLoacation, string textrueLocation, vec3 position);
	void setPosition(vec3 position);
	void update();
	void draw(GLuint& shaderProgram);
	GLuint m_vao;
	~Model();
private:
	vector<float> m_vertices;			// we can't initialise at compile time
	vector<float> m_normals;
	vector<float> m_textureUV;

	GLuint m_vboVertices;
	GLuint m_vboNormals;
	GLuint m_vboTextures;

	ModelLoader m_modelLoader;
	GLuint m_textureDataID;
	mat4 m_modelMatrix;

	float m_xAngle{ 0.0f };
	float m_yAngle{ 0.0f };
	float m_zAngle{ 0.0f };

	vec3 m_position;

};

