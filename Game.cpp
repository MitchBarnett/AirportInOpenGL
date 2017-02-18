#include "stdafx.h"
#include "Game.h"
#include "Model.h"
#include "Log.h"
#include "Mouse.h"


Game::Game()
{
	// we don't use RAII currently so no action in constructor
}

Game::~Game()
{
}

void Game::CreateGLWindow(HDC hdc, RECT rect, HWND window)
{
	m_window = window;
	m_win32OpenGL.CreateGLContext(hdc);	// may throw an exception!!!
										// MessageBoxA(0, (char*)glGetString(GL_VERSION), "OPENGL VERSION", 0);
	m_aspectRatio = static_cast<float>(rect.right / rect.bottom);
	m_win32OpenGL.CreateShadersAndProgram("PhongTexture");
	//m_win32OpenGL.CreateShadersAndProgram("phong");
	//m_win32OpenGL.CreateShadersAndProgram("flatVerticesAsColours");
	//m_win32OpenGL.CreateShadersAndProgram("diffuseOnly");
	//m_win32OpenGL.CreateShadersAndProgram("texture");
	RECT clientArea;
	POINT windowCenter;
	GetClientRect(m_window, &clientArea);
	windowCenter.x = clientArea.right / 2;
	windowCenter.y = clientArea.bottom / 2;
	m_lastMousePos = windowCenter;

	Mouse::setPosition(windowCenter, m_window);
}

void Game::DestroyGLWindow()
{
	m_win32OpenGL.TearDownGLContext();
}


void Game::PrepareToDraw()
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
	GLuint program = m_win32OpenGL.GetShaderProgram();
	Win32OpenGL::SendUniformMatrixToShader(program, m_projectionMatrix.m, "projection_matrix");
	m_MainCamera.setViewMatrix(program);

	m_mainLight.sendToShader(program);

	// now load in the model as with lighting


	m_models.push_back(new Model("Models\\simpleCubeWithTextures2.obj", "Textures\\redBrick256x256.bmp", vec3{ -1,-1,0 }));
	m_models.push_back(new Model("Models\\simpleCubeWithTextures2.obj", "Textures\\redBrick256x256.bmp", vec3{ 0,-0.5,0 }));
	m_models.push_back(new Model("Models\\simpleCubeWithTextures2.obj", "Textures\\redBrick256x256.bmp", vec3{ 1, 0, 0 }));
	m_models.push_back(new Model("Models\\simpleCubeWithTextures2.obj", "Textures\\redBrick256x256.bmp", vec3{ 2,0.5,0 }));
	//m_models.push_back(new Model("Models\\laptop.obj", "Textures\\laptop.bddd6mp", vec3{ 1,0.5,0 }));
	m_models.push_back(new Model("Models\\wood.obj", "Textures\\WoodTexture3.bmp", vec3{ -2,-1,0 }));
	m_models.push_back(new Model("Models\\oildrum.obj", "Textures\\oildrum.bmp", vec3{ -3,-1,0 }));
	m_models.push_back(new Model("Models\\groundPlane.obj", "Textures\\grass.bmp", vec3{ -1,-1,0 }));
}

void Game::Draw()
{
	m_win32OpenGL.ClearGLDisplay();
	GLuint program = m_win32OpenGL.GetShaderProgram();

	for (int i = 0; i < m_models.size(); i++)
	{
		m_models[i]->draw(program);
	}

	m_win32OpenGL.FinishedDrawing();
}

void Game::Update()
{
	m_MainCamera.update();
	GLuint program = m_win32OpenGL.GetShaderProgram();
	m_MainCamera.setViewMatrix(program);
	// we tumble the cube to see all the faces.
}
void Game::HandleMouse()
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
	GLuint program = m_win32OpenGL.GetShaderProgram();
	m_MainCamera.setViewMatrix(program);
}

void Game::HandleInput(unsigned char virtualKeyCode)
{
	// add code for interaction here
	//m_MainCamera.handleInput(virtualKeyCode);
	//GLuint program = m_win32OpenGL.GetShaderProgram();
	//m_MainCamera.setViewMatrix(program);

	/*if (virtualKeyCode == VK_UP)
	{

		m_cameraY += 0.1f;
		ComputeViewMatrix();
		GLuint program = m_win32OpenGL.GetShaderProgram();
		Win32OpenGL::SendUniformMatrixToShader(program, m_viewMatrix.m, "view_matrix");
	}
	else if (virtualKeyCode == VK_DOWN)
	{
		m_cameraY -= 0.1f;
		ComputeViewMatrix();
		GLuint program = m_win32OpenGL.GetShaderProgram();
		Win32OpenGL::SendUniformMatrixToShader(program, m_viewMatrix.m, "view_matrix");
	}

	else if (virtualKeyCode == VK_LEFT)
	{
		m_cameraX -= 0.1f;
		ComputeViewMatrix();
		GLuint program = m_win32OpenGL.GetShaderProgram();
		Win32OpenGL::SendUniformMatrixToShader(program, m_viewMatrix.m, "view_matrix");
	}
	else if (virtualKeyCode == VK_RIGHT)
	{
		m_cameraX += 0.1f;
		ComputeViewMatrix();
		GLuint program = m_win32OpenGL.GetShaderProgram();
		Win32OpenGL::SendUniformMatrixToShader(program, m_viewMatrix.m, "view_matrix");
	}
	else if (virtualKeyCode == VK_OEM_PLUS)
	{
		m_cameraZ -= 0.1f;
		ComputeViewMatrix();
		GLuint program = m_win32OpenGL.GetShaderProgram();
		Win32OpenGL::SendUniformMatrixToShader(program, m_viewMatrix.m, "view_matrix");
	}
	else if (virtualKeyCode == VK_OEM_MINUS)
	{
		m_cameraZ += 0.1f;
		ComputeViewMatrix();
		GLuint program = m_win32OpenGL.GetShaderProgram();
		Win32OpenGL::SendUniformMatrixToShader(program, m_viewMatrix.m, "view_matrix");
	}
	else if (virtualKeyCode == VK_NUMPAD4)
	{
		m_lightPosition.v[0] = -50.f;
		ComputeViewMatrix();
		GLuint program = m_win32OpenGL.GetShaderProgram();
		Win32OpenGL::SendUniformVector3ToShader(program, m_lightPosition.v, "light_position_world");
	}
	else if (virtualKeyCode == VK_NUMPAD6)
	{
		m_lightPosition.v[0] = 50.0f;
		ComputeViewMatrix();
		GLuint program = m_win32OpenGL.GetShaderProgram();
		Win32OpenGL::SendUniformVector3ToShader(program, m_lightPosition.v, "light_position_world");
	}
	else if (virtualKeyCode == VK_NUMPAD8)
	{
		m_lightPosition.v[1] = 50.0f;
		ComputeViewMatrix();
		GLuint program = m_win32OpenGL.GetShaderProgram();
		Win32OpenGL::SendUniformVector3ToShader(program, m_lightPosition.v, "light_position_world");
	}
	else if (virtualKeyCode == VK_NUMPAD2)
	{
		m_lightPosition.v[1] = -50.0f;
		ComputeViewMatrix();
		GLuint program = m_win32OpenGL.GetShaderProgram();
		Win32OpenGL::SendUniformVector3ToShader(program, m_lightPosition.v, "light_position_world");
	}

	else if (virtualKeyCode == VK_NUMPAD5)
	{
		m_lightPosition.v[0] = 0;
		m_lightPosition.v[1] = 0;
		ComputeViewMatrix();
		GLuint program = m_win32OpenGL.GetShaderProgram();
		Win32OpenGL::SendUniformVector3ToShader(program, m_lightPosition.v, "light_position_world");
	}

	else if (virtualKeyCode == VK_SPACE)
	{
		m_currentLight++;
		m_lightColourDiffuse = m_lightColours[m_currentLight % 4];
		GLuint program = m_win32OpenGL.GetShaderProgram();
		Win32OpenGL::SendUniformVector3ToShader(program, m_lightColourDiffuse.v, "light_colour_diffuse");

	}*/
}

void Game::setSensitivity(float sensitivity)
{
	m_MainCamera.m_lookSensitivity = sensitivity;
}

void Game::Resize(HDC hdc, RECT rect)
{
	// if the display is resized then OpenGL needs to know about the apect ratio
	// to compute the correct projection matrix
	m_win32OpenGL.Reshape(hdc, rect.right, rect.bottom);
	m_aspectRatio = (float)rect.right / rect.bottom;
	ComputeProjectionMatrix();
	GLuint program = m_win32OpenGL.GetShaderProgram();
	Win32OpenGL::SendUniformMatrixToShader(program, m_projectionMatrix.m, "projection_matrix");
}

void Game::ComputeProjectionMatrix()
{
	// we will look at this later in the course
	// in Modern OpenGL we must supply a projection matrix

#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444

	// input variables
	float zNear = 0.1f;		// clipping plane
	float zFar = 500.0f;	// clipping plane
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


bool Game::SimpleLoadTexture(string fileName, char* imageData)
{
	// we will replace this with code to read a BMP file and analyse the header in the next lab
	// for now we ASSUME a 256x256 pixel array of RGB data as unsigned bytes

	ifstream texFile;
	// important to ensure file is treated as binary data
	// char(28) is a file separator char
	// read in first texture
	texFile.open(fileName, ios::binary);
	if (!texFile)
	{
		cout << "error opening texture: " << fileName << endl;
		return false;
	}

	texFile.read(imageData, 256 * 256 * 3);
	texFile.close();
	return true;
}