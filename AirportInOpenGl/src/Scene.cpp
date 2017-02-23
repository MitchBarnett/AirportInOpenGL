#include "stdafx.h"
#include "Scene.h"
#include "Log.h"
#include <pugixml.hpp>
#include <string>
#include "Model.h"
Scene::Scene()
{
	load();
}

void Scene::load()
{
	m_models.insert(std::pair<string, Model>("errModel", Model("Models\\simpleCubeWithTextures2.obj", "Textures\\redBrick256x256.bmp")));
	pugi::xml_document doc;
	doc.load_file("Scenes//scene.xml");

	pugi::xml_node modelsTag = doc.child("scene").child("models");
	// tag::basic[]
	for (pugi::xml_node model = modelsTag.first_child(); model; model = model.next_sibling())
	{
		std::string name;
		std::string objFile;
		std::string textureFile;

		if (model.attribute("name").value() != "")
		{
			name = model.attribute("name").value();
		}
		if (model.attribute("objfile").value() != "")
		{
			objFile = model.attribute("objfile").value();
		}
		if (model.attribute("texturefile").value() != "")
		{
			textureFile = model.attribute("texturefile").value();
		}
		m_models.insert(std::pair<string,Model>(name, Model(objFile, textureFile)));
	}

	pugi::xml_node objectsTag = doc.child("scene").child("objects");
	// tag::basic[]
	for (pugi::xml_node object = objectsTag.first_child(); object; object = object.next_sibling())
	{
		Model*	model = &m_models["errModel"];
		vec3	position = { 0,0,0 };
		vec3	rotation = { 0,0,0 };
		vec3	scale = { 1,1,1 };
		if (m_models.find(object.attribute("model").value()) != m_models.end())
		{
			model = &m_models[object.attribute("model").value()];
		}

		m_objects.push_back(ModelInstance(model, position, rotation, scale));
	}
}

void Scene::draw(GLuint program)
{
	for (int i = 0; i < m_objects.size(); i++)
	{
		m_objects[i].draw(program);
	}
}
