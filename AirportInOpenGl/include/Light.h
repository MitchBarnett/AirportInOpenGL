#pragma once
#include "MathFunc.h"
#include "Win32OpenGl.h"

/*!
Representation of a light source in the scene.

Representation of a light source in the scene. The sendToShader function
must be called for the light to have any affect on the scene.
*/
class Light
{
public:
	/*!
	Default constructor.

	Creates a white light (1.0 specular, 0.8 diffuse, 0.3 ambient) at the origin equivalent to:
	\code
	Light(vec3{ 0.0f, 5.0f, 5.0f }, vec3{1.0f, 1.0f, 1.f},{ 0.8f, 0.8f, 0.8f }, vec3{ 0.3f, 0.3f, 0.3f })
	\endcode
	*/
	Light();
	/*!
	Construct a white light at a given position.

	Creates a white light (1.0 specular, 0.8 diffuse, 0.3 ambient) at the given position.
	\param position The position to place the light in the scene. \code vec3{x, y, z} \endcode
	*/
	Light(vec3 position);

	/*!
	Construct a custom light.

	Creates a light at a specified value.
	\param position The position to place the light in the scene. \code vec3{x, y, z} \endcode
	\param specular The specular component of the light. \code vec3{red, green, blue} \endcode 
	values in the range 0-1.
	\param diffuse The diffuse component of the light. \code vec3{red, green, blue} \endcode 
	values in the range 0-1.
	\param ambient The ambient component of the light. \code vec3{red, green, blue} \endcode
	values in the range 0-1.
	*/
	Light(vec3 position, vec3 specular, vec3 diffuse, vec3 ambient);

	/*!
	Sets the position of the light.
	\param position The new position of the light. \code vec3{red, green, blue} \endcode
	values in the range 0-1.
	*/
	void setPosition(vec3 position);
	/*!
	Sets the specular component of the light.
	\param specular The new specular component of the light. \code vec3{red, green, blue} \endcode
	values in the range 0-1.
	*/
	void setSpecular(vec3 specular);

	/*!
	Sets the diffuse component of the light.
	\param diffuse The new diffuse component of the light. \code vec3{red, green, blue} \endcode
	values in the range 0-1.
	*/
	void setDiffuse(vec3 diffuse);

	/*!
	Sets the ambient component of the light.
	\param ambient The new ambient component of the light. \code vec3{red, green, blue} \endcode
	values in the range 0-1.
	*/
	void setAmbient(vec3 ambient);

	/*!
	Sends the lights components to the provided shader program.
	\param program The shader program to send the light to.
	*/
	void sendToShader(GLuint program);


private:
	// Given default values
	vec3 m_specular{ 1.0f,1.0f,1.0f };		///< Specular component of the light.
	vec3 m_diffuse{ 0.8f, 0.8f, 0.8f };		///< Diffuse component of the light.
	vec3 m_ambient{ 0.3f, 0.3f, 0.3f };		///< Ambient component of the light.
	vec3 m_position{ 0.0f, 0.0f, 0.0f };	///< The position of the light.

};

