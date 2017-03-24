#include "stdafx.h"
#include "Program.h"
#include "Model.h"
#include "Log.h"
#include "Mouse.h"
#include "Scene.h"


Program::Program()
{
	// we don't use RAII currently so no action in constructor
}

Program::~Program()
{
}

void Program::CreateGLWindow(HDC hdc, RECT rect, HWND window)
{
	m_window = window;
	m_win32OpenGL.CreateGLContext(hdc);	// may throw an exception!!!
										// MessageBoxA(0, (char*)glGetString(GL_VERSION), "OPENGL VERSION", 0);
	m_aspectRatio = static_cast<float>(rect.right / rect.bottom);

	RECT clientArea;
	POINT windowCenter;
	GetClientRect(m_window, &clientArea);
	windowCenter.x = clientArea.right / 2;
	windowCenter.y = clientArea.bottom / 2;
	m_lastMousePos = windowCenter;

	Mouse::setPosition(windowCenter, m_window);
}

void Program::DestroyGLWindow()
{
	m_win32OpenGL.TearDownGLContext();
}


void Program::PrepareToDraw()
{
	
	m_lightColours.push_back(vec3{ 1.0f, 0.0f, 0.0f });
	m_lightColours.push_back(vec3{ 0.0f, 1.0f, 0.0f });
	m_lightColours.push_back(vec3{ 0.0f, 0.0f, 1.0f });
	m_lightColours.push_back(vec3{ 1.0f, 1.0f, 1.0f });

	ComputeProjectionMatrix();
	m_MainCamera.computeViewMatrixUsingLookAt();
	// we compute the model matrix in the draw routine so we can animate the
	// triangle

	// send the matrixes to the shader
	//GLuint program = m_win32OpenGL.GetShaderProgram();
	GLuint a;
	GLuint b;
	Win32OpenGL::CreateShadersAndProgram("PhongTexture", m_phongShader);
	Win32OpenGL::SendUniformMatrixToShader(m_phongShader, m_projectionMatrix.m, "projection_matrix");
	m_MainCamera.setViewMatrix(m_phongShader);
	m_mainLight.sendToShader(m_phongShader);

	Win32OpenGL::CreateShadersAndProgram("texture", m_unlitShader);
	Win32OpenGL::SendUniformMatrixToShader(m_unlitShader, m_projectionMatrix.m, "projection_matrix");
	m_MainCamera.setViewMatrix(m_unlitShader);
	m_mainLight.sendToShader(m_unlitShader);
	
	Scene loading("Scenes\\loading.xml");
	Win32OpenGL::UseProgram(m_unlitShader);
	loading.draw(m_unlitShader);
	m_win32OpenGL.FinishedDrawing();
	RedrawWindow(m_window, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	// now load in the model as with lighting

	m_scene = new Scene("Scenes\\loading.xml");

	m_models.push_back(new Model("Models\\ground.obj", "Textures\\grass2.bmp"));
	m_models.push_back(new Model("Models\\cube.obj", "Textures\\SkyBox.bmp"));
	m_objects.push_back(new Object(m_models[0], vec3{ 0,0,0 }, vec3{ 0,0,0 }, vec3{ 1,1,1 }));
	m_objects.push_back(new Object(m_models[1], vec3{ 0,0,0 }, vec3{ 0,0,0 }, vec3{ 4,4,4 }));
	Win32OpenGL::UseProgram(m_phongShader);
}

void Program::loadScene()
{
	m_scene->load("Scenes\\scene.xml");
}

void Program::reloadScene()
{
	m_loaded = false;
	m_scene->load("Scenes\\loading.xml");
	Draw();
	RedrawWindow(m_window, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	m_scene->load("Scenes\\scene.xml");
	m_loaded = true;
	Draw();
}

void Program::Draw()
{
	m_win32OpenGL.ClearGLDisplay();
	if (m_loaded)
	{
		m_objects[0]->draw(m_unlitShader);
		m_objects[1]->draw(m_unlitShader);
		m_scene->draw(m_phongShader);
	}
	else
	{
		m_scene->draw(m_unlitShader);
	}	
	m_win32OpenGL.FinishedDrawing();
}

void Program::Update()
{
	if (GetAsyncKeyState('P'))
	{
		Log::AppendToLogFile("Position logged...");
		Log::AppendToLogFile("X" + to_string(m_MainCamera.m_position.v[0]));
		Log::AppendToLogFile("Y" + to_string(m_MainCamera.m_position.v[1]));
		Log::AppendToLogFile("Z" + to_string(m_MainCamera.m_position.v[2]));		
	}
	m_MainCamera.update();
	m_objects[m_objects.size()-1]->setPosition(m_MainCamera.m_position);
	m_MainCamera.setViewMatrix(m_phongShader);
	m_MainCamera.setViewMatrix(m_unlitShader);
	// we tumble the cube to see all the faces.
}
void Program::HandleMouse()
{
	m_currentMousePos = Mouse::getPosition(m_window);

	POINT difference;
	difference.x = m_currentMousePos.x - m_lastMousePos.x;
	difference.y = m_currentMousePos.y - m_lastMousePos.y;

	m_MainCamera.handleInput(difference.x, difference.y);
	m_lastMousePos = m_currentMousePos;

	if (difference.x != 0 || difference.y != 0)
	{
		RECT clientArea;
		POINT windowCenter;
		GetClientRect(m_window, &clientArea);
		windowCenter.x = clientArea.right / 2;
		windowCenter.y = clientArea.bottom / 2;
		m_lastMousePos = windowCenter;

		Mouse::setPosition(windowCenter, m_window);
	}
	m_MainCamera.handleInput(difference.x, difference.y);
	m_MainCamera.setViewMatrix(m_phongShader);
	m_MainCamera.setViewMatrix(m_unlitShader);
}

void Program::setSensitivity(float sensitivity)
{
	m_MainCamera.m_lookSensitivity = sensitivity;
}

void Program::Resize(HDC hdc, RECT rect)
{
	// if the display is resized then OpenGL needs to know about the apect ratio
	// to compute the correct projection matrix
	m_win32OpenGL.Reshape(hdc, rect.right, rect.bottom);
	m_aspectRatio = (float)rect.right / rect.bottom;
	ComputeProjectionMatrix();
	Win32OpenGL::SendUniformMatrixToShader(m_phongShader, m_projectionMatrix.m, "projection_matrix");
	Win32OpenGL::SendUniformMatrixToShader(m_unlitShader, m_projectionMatrix.m, "projection_matrix");
}

void Program::ComputeProjectionMatrix()
{
	// we will look at this later in the course
	// in Modern OpenGL we must supply a projection matrix

#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444

	// input variables
	float zNear = 0.1f;		// clipping plane
	float zFar = 1500.0f;	// clipping plane
	float fov = static_cast<float>(67.0f * ONE_DEG_IN_RAD); // convert 67 degrees to radians

	float range = tan(fov * 0.5f) * zNear;
	float Sx = (2.0f * zNear) / (range * m_aspectRatio + range * m_aspectRatio);
	float Sy = zNear / range;
	float Sz = -(zFar + zNear) / (zFar - zNear);
	float Pz = -(2.0f * zFar * zNear) / (zFar - zNear);
	GLfloat proj_mat[] = {
		Sx, 0.0f, 0.0f, 0.0f,
		0.0f, Sy, 0.0f, 0.0f,
		0.0f, 0.0f, Sz, -1.0f,
		0.0f, 0.0f, Pz, 0.0f
	};
	for (int i = 0; i < 16; i++)
	{
		m_projectionMatrix.m[i] = proj_mat[i];
	}
}

