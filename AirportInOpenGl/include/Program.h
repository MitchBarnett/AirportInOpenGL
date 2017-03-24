#pragma once
#include "Win32OpenGL.h"
#include "Log.h"
#include "Model.h"
#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include <Windows.h>
#include <gl\GL.h>
#include "Scene.h"

class Program
{
	// open gl helper class
	Win32OpenGL m_win32OpenGL;
	HWND m_window;
	// matrices
	mat4 m_projectionMatrix;

	vector<vec3> m_lightColours;
	POINT m_currentMousePos;
	POINT m_lastMousePos;

	int m_currentLight = 3;

	float m_aspectRatio;

	Camera m_MainCamera;
	Light m_mainLight{ vec3{ 0.0f, 30.0f, 20.0f } };

	GLuint m_phongShader;
	GLuint m_unlitShader;

	Scene*	m_scene;
	

	vector<Model*> m_models;
	vector<Object*> m_objects;

public:
	Program();
	~Program();
	bool m_loaded = false;
	void CreateGLWindow(HDC hdc, RECT rect, HWND window);
	void DestroyGLWindow();

	void PrepareToDraw();
	void loadScene();
	void reloadScene();
	void Draw();

	void Update();

	void HandleMouse();
	void setSensitivity(float sensitivity);

	void Resize(HDC hdc, RECT rect);

	void ComputeProjectionMatrix();

};

