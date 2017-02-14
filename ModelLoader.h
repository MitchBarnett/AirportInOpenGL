#pragma once
#include "MathFunc.h"
#include <vector>
#include <string>
class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();
	bool genModel(std::string filename);
	std::vector<float> getVertices();
	std::vector<float> getNormals();
	std::vector<float> getTextureUVs();

private:
	//vec3								m_vertex;					// Holds the x,y,z components of the current vertex		
	//vec2								m_texVertex;				// Holds the u,v components of the current texture vertex
	//vec3								m_vertexNormal;				// Holds the i, j ,k components of the current normal

	std::vector<vec3>					m_vertices;				// Holds all the vertices
	std::vector<vec3>					m_vertNormals;			// Holds all the normal vectors
	std::vector<vec2>					m_texCords;				// Holds all the texture verticies

	std::vector<int>					m_vertexIndices;		// Holds the indicies of a vertex to make up a face
	std::vector<int>					m_normalIndicies;		// Holds the indicies of a vertex Normal to make up a face
	std::vector<int>					m_texIndicies;			// Holds the indicies of a vertex to make up a face

	std::vector<float>					m_modelVertices;		// Holds the final vertices to be passed to the vao
	std::vector<float>					m_modelNormals;			// Holds the final vertex normals
	std::vector<float>					m_modelTexCords;		// Holds the final textrue cordinates 

};

