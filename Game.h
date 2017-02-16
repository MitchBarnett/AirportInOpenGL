#pragma once
#include "Win32OpenGL.h"
#include "resource.h"
#include "Log.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include <Windows.h>
//#include "Light.h"

class Game
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
	Light m_mainLight{ vec3{ 0.0f, 5.0f, 5.0f } };

	vector<Model*> m_models;

public:
	Game();
	~Game();

	void CreateGLWindow(HDC hdc, RECT rect, HWND window);
	void DestroyGLWindow();

	void PrepareToDraw();
	void Draw();

	void Update();

	void HandleMouse();
	void HandleInput(unsigned char virtualKeyCode);
	void setSensitivity(float sensitivity);

	void Resize(HDC hdc, RECT rect);

	void ComputeProjectionMatrix();

	bool SimpleLoadTexture(string fileName, char* imageData);
};

