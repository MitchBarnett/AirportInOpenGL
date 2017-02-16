#pragma once
#include "Win32OpenGL.h"
#include "resource.h"
#include "Log.h"
#include "Model.h"
#include "Camera.h"
#include <Windows.h>
//#include "Light.h"

class Game
{
	// open gl helper class
	Win32OpenGL m_win32OpenGL;
	Camera m_MainCamera;
	HWND m_window;
	// matrices
	mat4 m_projectionMatrix;

	vector<vec3> m_lightColours;
	POINT m_currentMousePos;
	POINT m_lastMousePos;

	int m_currentLight = 3;

	float m_aspectRatio;

	// and this is from the lighting
	vec3 m_lightColourSpecular{ 1.0f,1.0f,1.0f };
	vec3 m_lightColourDiffuse{ 0.8f, 0.8f, 0.8f };
	vec3 m_lightColourAmbient{ 0.3f, 0.3f, 0.3f };
	vec3 m_lightPosition{ 0.0f, 5.0f, 5.0f };

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

