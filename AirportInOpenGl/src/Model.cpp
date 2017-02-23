#include "stdafx.h"
#include "Model.h"

Model::Model()
{
}

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


void Model::draw()
{
	glBindTexture(GL_TEXTURE_2D, m_textureDataID);
	glBindVertexArray(m_vao);		// select first VAO
	GLuint numberOfElements = m_vertices.size() / 3;
	glDrawArrays(GL_TRIANGLES, 0, numberOfElements);
	glBindVertexArray(NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

Model::~Model()
{
}
