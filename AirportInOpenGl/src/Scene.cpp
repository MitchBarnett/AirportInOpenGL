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
	pugi::xml_document doc;
	doc.load_file("Scenes//scene.xml");
	pugi::xml_node modelsTag = doc.child("scene").child("models");
	// tag::basic[]
	for (pugi::xml_node model = modelsTag.first_child(); model; model = model.next_sibling())
	{
		std::string objFile;
		std::string textureFile;

		for (pugi::xml_attribute attr = model.first_attribute(); attr; attr = attr.next_attribute())
		{
			if (string(attr.name()) == "objfile")
			{
				objFile = attr.value();
			}
			else if (string(attr.name()) == "texturefile")
			{
				textureFile = attr.value();
			}
		}
		m_models.push_back(new Model(objFile, textureFile));
	}
}
