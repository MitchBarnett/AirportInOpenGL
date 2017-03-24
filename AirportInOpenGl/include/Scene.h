#pragma once
#include <vector>
#include <map>
#include "Object.h"

/*!
Controls a scene of objects, loads values from an XML file.

Loads in models and objects through XML has functions to load update and draw 
a scene.
*/
class Scene
{
public:
	/*!
	Default constructor, loads scene.xml.

	Creates a scene by loading it in through XML.
	*/
	Scene();

	/*!
	secondary constructor, loads a provided scene.

	Creates a scene by loading it in through XML.
	*/
	Scene(string file);

	/*!
	Loads in a scene through XML.

	Loads in models and objects through XML.
	Creates models from .obj and .bmp files specified in the xml file.
	Creates objects from a given model name and position, rotation and scale
	infomation. All atributes are optional for maxium flexibilty and ease of use.
	*/
	void load(string file);

	/*!
	Updates the scene (Not currently implemented).
	*/
	void update();

	/*!
	Draws the scene.

	Runs through each object in the object vector and draws it.
	*/
	void draw(GLuint program);

private:
	string m_filename;
	map<string, Model> m_models;  //!< Map of models has a unique name string as a key.
	vector<Object> m_objects; //!< Vector of objects used to draw and update them.
};

