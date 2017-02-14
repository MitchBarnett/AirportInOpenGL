#include "stdafx.h"
#include "Model.h"

Model::Model(string modelLocation, string textrueLocation)
{
	if (!m_modelLoader.genModel(modelLocation))
	{
		Log::AppendToLogFile("Model file not found");
		throw std::invalid_argument("Model file not found");
		return;
	}

	m_vertices = m_modelLoader.getVertices();
	m_normals = m_modelLoader.getNormals();
	m_textureUV = m_modelLoader.getTextureUVs();

	glGenBuffers(1, &m_vboVertices);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(GLfloat), &m_vertices[0], GL_STATIC_DRAW);

	//GLuint vboNormals;
	glGenBuffers(1, &m_vboNormals);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboNormals);
	glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(GLfloat), &m_normals[0], GL_STATIC_DRAW);

	//GLuint vboTextures;
	glGenBuffers(1, &m_vboTextures);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboTextures);
	glBufferData(GL_ARRAY_BUFFER, m_textureUV.size() * sizeof(GLfloat), &m_textureUV[0], GL_STATIC_DRAW);

	//GLuint vao;
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboNormals);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboTextures);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	// vertices are element 0 in VAO
	// normals element 1
	// textures element 2
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	TextureLoader::LoadBMP(textrueLocation, m_textureDataID);
}

Model::Model(string modelLocation, string textrueLocation, vec3 position)
{
	if (!m_modelLoader.genModel(modelLocation))
	{
		Log::AppendToLogFile("Model file not found");
		throw std::invalid_argument("Model file not found");
	}

	m_vertices = m_modelLoader.getVertices();
	m_normals = m_modelLoader.getNormals();
	m_textureUV = m_modelLoader.getTextureUVs();

	glGenBuffers(1, &m_vboVertices);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(GLfloat), &m_vertices[0], GL_STATIC_DRAW);

	//GLuint vboNormals;
	glGenBuffers(1, &m_vboNormals);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboNormals);
	glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(GLfloat), &m_normals[0], GL_STATIC_DRAW);

	//GLuint vboTextures;
	glGenBuffers(1, &m_vboTextures);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboTextures);
	glBufferData(GL_ARRAY_BUFFER, m_textureUV.size() * sizeof(GLfloat), &m_textureUV[0], GL_STATIC_DRAW);

	//GLuint vao;
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboNormals);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboTextures);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	// vertices are element 0 in VAO
	// normals element 1
	// textures element 2
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	TextureLoader::LoadBMP(textrueLocation, m_textureDataID);

	m_position = position;
}

void Model::setPosition(vec3 position)
{
	m_position = position;
}

void Model::draw(GLuint& shaderProgram)
{
	glBindTexture(GL_TEXTURE_2D, m_textureDataID);
	m_modelMatrix = identity_mat4();
	m_modelMatrix = translate(m_modelMatrix, m_position);
	m_modelMatrix = rotate_x_deg(m_modelMatrix, m_xAngle);
	m_modelMatrix = rotate_y_deg(m_modelMatrix, m_yAngle);
	m_modelMatrix = rotate_z_deg(m_modelMatrix, m_zAngle);
	Win32OpenGL::SendUniformMatrixToShader(shaderProgram, m_modelMatrix.m, "model_matrix");

	glBindVertexArray(m_vao);		// select first VAO
	GLuint numberOfElements = m_vertices.size() / 3;
	glDrawArrays(GL_TRIANGLES, 0, numberOfElements);
	glBindVertexArray(NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

Model::~Model()
{
}
