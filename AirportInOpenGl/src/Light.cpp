#include "stdafx.h"
#include "Light.h"
#include "Win32OpenGl.h"


Light::Light()
{
}

Light::Light(vec3 position) 
:m_position(position)
{ }

Light::Light(vec3 position, vec3 specular, vec3 diffuse, vec3 ambient) 
: m_position	{ position },
m_specular		{ specular },
m_diffuse		{ diffuse },
m_ambient		{ ambient } 
{ }

void Light::sendToShader(GLuint program)
{
	Win32OpenGL::SendUniformVector3ToShader(program, m_position.v, "light_position_world");
	Win32OpenGL::SendUniformVector3ToShader(program, m_specular.v, "light_colour_specular");
	Win32OpenGL::SendUniformVector3ToShader(program, m_diffuse.v, "light_colour_diffuse");
	Win32OpenGL::SendUniformVector3ToShader(program, m_ambient.v, "light_colour_ambient");
}

