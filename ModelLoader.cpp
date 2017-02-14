#pragma once
#include "stdafx.h"
#include "ModelLoader.h"
#include <fstream>
#include <sstream> 


ModelLoader::ModelLoader()
{

}


ModelLoader::~ModelLoader()
{
}

bool ModelLoader::genModel(std::string filename)
{
	std::ifstream modelFile(filename, std::ios_base::in);
	static const int forwardSlash = 47;

	if (!modelFile)
	{
		return false;
	}
	std::string currentLine;
	
	std::string s;
	int lookAhead;
	while (getline(modelFile, currentLine))
	{
		std::istringstream iss(currentLine);
		iss >> s;

		if (s == "v")
		{
			float f1, f2, f3;
			iss >> f1;
			iss >> f2;
			iss >> f3;
			vec3 vertex(f1, f2, f3);
			m_vertices.push_back(vertex);
		}

		if (s == "vn")
		{
			float f1, f2, f3;
			iss >> f1;
			iss >> f2;
			iss >> f3;
			vec3 VertexNormal(f1, f2, f3);
			m_vertNormals.push_back(VertexNormal);
		}

		if (s == "vt")
		{
			float f1, f2;
			iss >> f1;
			iss >> f2;
			vec2 textureVertex(f1, f2);
			m_texCords.push_back(textureVertex);
		}

		if (s == "f")
		{
			int vertexIndex;
			int normalIndex;
			int textureIndex;
			while (iss >> vertexIndex)
			{
				vertexIndex--;
				m_vertexIndices.push_back(vertexIndex);
				lookAhead = iss.peek();

				if (lookAhead == forwardSlash)	//check for first slash
				{
					iss.get();
					lookAhead = iss.peek();
					if (lookAhead == forwardSlash)	// check for second slash. just normals
					{
						iss.get();
						iss >> normalIndex;
						normalIndex--;
						m_normalIndicies.push_back(normalIndex);
					}
					else // normals and uv's
					{
						iss >> textureIndex;
						textureIndex--;
						m_texIndicies.push_back(textureIndex);
						lookAhead = iss.get();
						if (lookAhead == forwardSlash)
						{
							iss >> normalIndex;
							normalIndex--;
							m_normalIndicies.push_back(normalIndex);
						}
					}
				}
			}
		}
	}

	for (unsigned int i = 0; i < m_vertexIndices.size(); i++)
	{
		vec3 vertex = m_vertices[m_vertexIndices[i]];
		m_modelVertices.push_back(vertex.v[0]);
		m_modelVertices.push_back(vertex.v[1]);
		m_modelVertices.push_back(vertex.v[2]);
	}

	for (unsigned int i = 0; i < m_normalIndicies.size(); i++)
	{
		vec3 vertexNormal = m_vertNormals[m_normalIndicies[i]];
		m_modelNormals.push_back(vertexNormal.v[0]);
		m_modelNormals.push_back(vertexNormal.v[1]);
		m_modelNormals.push_back(vertexNormal.v[2]);
	}

	for (unsigned int i = 0; i < m_texIndicies.size(); i++)
	{
		vec2 textureUV = m_texCords[m_texIndicies[i]];
		m_modelTexCords.push_back(textureUV.v[0]);
		m_modelTexCords.push_back(textureUV.v[1]);
	}
	return true;
}

std::vector<float> ModelLoader::getVertices()
{
	return m_modelVertices;
}

std::vector<float> ModelLoader::getNormals()
{
	return m_modelNormals;
}

std::vector<float> ModelLoader::getTextureUVs()
{
	return m_modelTexCords;
}

