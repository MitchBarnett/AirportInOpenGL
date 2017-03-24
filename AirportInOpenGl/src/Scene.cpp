#include "stdafx.h"
#include "Scene.h"
#include <pugixml.hpp>
#include "Log.h"
#include <string>
#include "Model.h"
Scene::Scene()
{
	load("Scenes\\scene.xml");	// Load in the scene
}

Scene::Scene(string file)
{
	load(file);	// Load in the scene
}

void Scene::load(string file)
{
	Log::AppendToLogFile("==== Scene loading starting ====");
	
	// Clear the vector so if load is called as a reload models are not drawn multiple times.
	m_models = {};
	m_objects = {};

	// A default model to be used if creating a object fails. 
	m_models.insert(std::pair<string, Model>("errModel", Model("Models\\simpleCubeWithTextures2.obj", "Textures\\errTex.bmp")));

	// Parse the XML document into DOM format
	pugi::xml_document doc;
	pugi::xml_parse_result parseResult = doc.load_file(file.c_str());

	if (parseResult)	// Parsed without errors
	{
		Log::AppendToLogFile(file + " file was parsed without errors");
	}
	else // Parsed with errors
	{
		Log::AppendToLogFile(file +" file failed to parse");
		Log::AppendToLogFile("parsing error:" + string(parseResult.description()));
		Log::AppendToLogFile("Run through a XML validator");
	}

	// modelsTag in XML signifies the start of the list of models
	pugi::xml_node modelsTag = doc.child("scene").child("models");

	// For each model in the XML file.
	for (pugi::xml_node model = modelsTag.first_child(); model; model = model.next_sibling())
	{
		// Deafult values to be used if there is an error getting them from the file.
		std::string name = "Error";
		std::string objFile = "Models\\simpleCubeWithTextures2.obj";
		std::string textureFile = "Textures\\errTex.bmp";

		// try to get the texture and obj of the model from the file.
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

		// Add a model with the given attributes into the models map.
		m_models.insert(std::pair<string,Model>(name, Model(objFile, textureFile)));
	}
	// Log the number of models loaded.
	Log::AppendToLogFile(to_string(m_models.size()) + " Models loaded");

	// modelsTag in XML signifies the start of the list of objects
	pugi::xml_node objectsTag = doc.child("scene").child("objects");
	int objectCount = 0; // used to count the number of models attempted to load in.

	// for each object in the XML file.
	for (pugi::xml_node object = objectsTag.first_child(); object; object = object.next_sibling())
	{
		objectCount++; // increment as new object load attempted.

		// Deafult values to be used if there is an error getting them from the file.
		Model*	model = &m_models["errModel"];
		vec3	position = { 0,0,0 };
		vec3	rotation = { 0,0,0 };
		vec3	scale = { 1,1,1 };

		// Checks there is a model attribute in the xml file and checks
		// that there is a model with the given name in the models map.
		if (m_models.find(object.attribute("model").value()) != m_models.end())
		{
			model = &m_models[object.attribute("model").value()];
		}
		// If not log that an error loading the object occured.
		else
		{
			Log::AppendToLogFile("Creating object #: " + to_string(objectCount) + "\" failed");
			Log::AppendToLogFile("no model named: \"" + string(object.attribute("model").value()) + "\" found");
		}

		//  Find nodes for the position, rotation and scale elements of the object.
		pugi::xml_node positionNode = object.child("position");
		pugi::xml_node rotationNode = object.child("rotation");
		pugi::xml_node scaleNode = object.child("scale");


		bool errorFlag = false;	// Will be set true if an error occurs

		/*
		The following will check if there are nodes for position, rotation and scale
		for the object. If the the node is found the relevent attributes will then be searched for
		the element will be converted into a float and will change the appropiate value in the vec3
		for that node. It is wrapped in try/catch blocks as stof will throw and exception if the
		value cannot be converted to a float.		
		*/
		if (positionNode)	// Check if there is a position node.
		{
			if (positionNode.attribute("x").value() != "")
			{
				try { position.v[0] = stof(positionNode.attribute("x").value()); }
				catch (std::invalid_argument& e) { errorFlag = true; }
			}
			if (positionNode.attribute("y").value() != "")
			{
				try { position.v[1] = stof(positionNode.attribute("y").value()); }
				catch (std::invalid_argument& e) { errorFlag = true; }
			}
			if (positionNode.attribute("z").value() != "")
			{
				try { position.v[2] = stof(positionNode.attribute("z").value()); }
				catch (std::invalid_argument& e) { errorFlag = true; }
			}
		}

			if (rotationNode) // Check if there is a roatation node.
			{
				if (rotationNode.attribute("pitch").value() != "")
				{
					try { rotation.v[0] = stof(rotationNode.attribute("pitch").value()); }
					catch (std::invalid_argument& e) { errorFlag = true; }
				}
				if (rotationNode.attribute("yaw").value() != "")
				{
					try { rotation.v[1] = stof(rotationNode.attribute("yaw").value()); }
					catch (std::invalid_argument& e) { errorFlag = true; }
				}
				if (rotationNode.attribute("roll").value() != "")
				{
					try { rotation.v[2] = stof(rotationNode.attribute("roll").value()); }
					catch (std::invalid_argument& e) { errorFlag = true; }
				}
			}

			if (scaleNode) // Check if there is a scale node.
			{
				if (scaleNode.attribute("x").value() != "")
				{
					try { scale.v[0] = stof(scaleNode.attribute("x").value()); }
					catch (std::invalid_argument& e) { errorFlag = true; }
				}
				if (scaleNode.attribute("y").value() != "")
				{
					try { scale.v[1] = stof(scaleNode.attribute("y").value()); }
					catch (std::invalid_argument& e) { errorFlag = true; }
				}
				if (scaleNode.attribute("z").value() != "")
				{
					try { scale.v[2] = stof(scaleNode.attribute("z").value()); }
					catch (std::invalid_argument& e) { errorFlag = true; }
				}
			}
			if (errorFlag)	// If an error occured while converting values log it.
			{
				Log::AppendToLogFile("Creating object #: " + to_string(objectCount) + " with model \"" + string(object.attribute("model").value()) + "\" error occurred");
				Log::AppendToLogFile("one or more values could not be converted to float type");
			}
			// Create a new object with the values loaded from the file and add it to the objects vector.
			m_objects.push_back(ModelInstance(model, position, rotation, scale));
		}

	// Log the number of objects loaded and the number of objects found in the xml file.
	Log::AppendToLogFile(to_string(m_objects.size()) + "/" + to_string(objectCount) + " Objects loaded");
	Log::AppendToLogFile("==== Scene loading complete ====");
}

void Scene::draw(GLuint program)
{
	// For each object in the vector draw it with a given shader.
	for (int i = 0; i < m_objects.size(); i++)
	{
		m_objects[i].draw(program);
	}
}
