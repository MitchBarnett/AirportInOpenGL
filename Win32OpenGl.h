/*

Win32OpenGL.h

*/
#pragma once
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

//--- OpenGL --- 

// we don't want to have to use the glew.dll so set static and link to glews.lib
#define GLEW_STATIC

#include "glew.h"
#include "wglew.h"


#include <windows.h>		//before gl.h?
#include <GL/GL.h>

// anton matrix and vector libraries
#include "MathFunc.h"

// c  and stl libraries
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Log.h"

using namespace std;

class Win32OpenGL
{
public:

	Win32OpenGL();
	virtual ~Win32OpenGL();

	bool CreateGLContext(HDC hdc); 	// Creates OpenGL Rendering Context
	void TearDownGLContext();
	void CreateShadersAndProgram(string baseName);

	// static version can be called by shader manager
	static void Win32OpenGL::CreateShadersAndProgram(string baseName, GLuint& program);
	static void Win32OpenGL::UseProgram(GLuint program);

	void Reshape(HDC hdc, int w, int h);	// Changing viewport

	GLuint GetShaderProgram();		//

									// static functions - no data preserved
	static void SendUniformMatrixToShader(GLuint program, float matrixData[16], std::string uniformName);
	static void SendUniformVector3ToShader(GLuint program, float vector[3], std::string uniformName);
	static void CreateVAO(GLuint& vao, GLuint& vboVertices, GLuint& vboNormals, vector<float>& vertices, vector<float>& normals);
	static void CreateVAO(GLuint& vao, GLuint& vboVertices, vector<float>& vertices);					// only vertices!
	static void CreateVAOWithTextures(GLuint&, GLuint&, GLuint&, GLuint&, vector<float>&, vector<float>&, vector<float>&);

	static void SetupDisplay();
	static void ClearGLDisplay();
	static void Draw(GLuint vao, int numberOfElements);
	static void DrawLines(GLuint vao, int numberOfElements);
	static void FinishedDrawing();
	static void DestroyVAO(GLuint& vao, GLuint& vboVertices, GLuint& vboNormals);
	static void DestroyVAO(GLuint& vao, GLuint& vboVertices);
	static void DestroyVAOWithTextures(GLuint& vao, GLuint& vboVertices, GLuint& vboNormals, GLuint& vboTextures);


protected:

	// shader stuff
	static char* ParseFileIntoString(std::string file_name);

	static bool CreateShader(const char* file_name, GLuint* shader, GLenum type);
	static void LogShaderInfo(GLuint shader_index, GLenum type);

	static bool CreateProgram(GLuint vert, GLuint frag, GLuint* program);
	static bool IsProgramValid(GLuint sp);
	static void LogProgramInfo(GLuint sp);

	HGLRC m_hrc;                        // OpenGL Rendering Context 

	GLuint m_program;					// the shader program
	GLuint m_vertexShader;
	GLuint m_fragmentShader;

};

