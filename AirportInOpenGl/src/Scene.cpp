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
	m_models = {};
	m_objects = {};
	m_models.insert(std::pair<string, Model>("errModel", Model("Models\\simpleCubeWithTextures2.obj", "Textures\\redBrick256x256.bmp")));
	pugi::xml_document doc;
	pugi::xml_parse_result parseResult = doc.load_file("Scenes\\scene.xml");

	if (parseResult)
	{
		Log::AppendToLogFile("Scenes\\scene.xml file was parsed without errors");
	}
	else
	{
		Log::AppendToLogFile("Scenes//scene.xml file failed to parse");
		Log::AppendToLogFile("parsing error:" + string(parseResult.description()));
	}
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
	Log::AppendToLogFile(to_string(m_models.size()) + " Models loaded");

	pugi::xml_node objectsTag = doc.child("scene").child("objects");
	// tag::basic[]
	int objectCount = 0;
	for (pugi::xml_node object = objectsTag.first_child(); object; object = object.next_sibling())
	{
		objectCount++;
		Model*	model = &m_models["errModel"];
		vec3	position = { 0,0,0 };
		vec3	rotation = { 0,0,0 };
		vec3	scale = { 1,1,1 };
		if (m_models.find(object.attribute("model").value()) != m_models.end())
		{
			model = &m_models[object.attribute("model").value()];
		}
		else
		{
			Log::AppendToLogFile("Creating object #: " + to_string(objectCount) + "\" failed");
			Log::AppendToLogFile("no model named: \"" + string(object.attribute("model").value()) + "\" found");
		}
		pugi::xml_node positionNode = object.child("position");
		pugi::xml_node rotationNode = object.child("rotation");
		pugi::xml_node scaleNode = object.child("scale");

		try
		{
			if (positionNode)
			{
				position.v[0] = stof(positionNode.attribute("x").value());
				position.v[1] = stof(positionNode.attribute("y").value());
				position.v[2] = stof(positionNode.attribute("z").value());
			}

			if (rotationNode)
			{
				rotation.v[0] = stof(rotationNode.attribute("pitch").value());
				rotation.v[1] = stof(rotationNode.attribute("roll").value());
				rotation.v[2] = stof(rotationNode.attribute("yaw").value());
			}

			if (scaleNode)
			{
				scale.v[0] = stof(scaleNode.attribute("x").value());
				scale.v[1] = stof(scaleNode.attribute("y").value());
				scale.v[2] = stof(scaleNode.attribute("z").value());
			}
		}
		catch(std::invalid_argument& e)
		{
			Log::AppendToLogFile("Creating object #: " + to_string(objectCount) +  " with model \"" + string(object.attribute("model").value()) + "\" failed");
			Log::AppendToLogFile("one or more values could not be converted to float type");
		}
		m_objects.push_back(ModelInstance(model, position, rotation, scale));
	}
	Log::AppendToLogFile(to_string(m_objects.size()) + " Objects loaded");
}

void Scene::draw(GLuint program)
{
	for (int i = 0; i < m_objects.size(); i++)
	{
		m_objects[i].draw(program);
	}
}
