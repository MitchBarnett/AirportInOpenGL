#pragma once
#include <vector>
#include <map>
#include "ModelInstance.h"
#
class Scene
{
public:
	Scene();
	void load();
	void update();
	void draw(GLuint program);
private:
	map<string, Model> m_models;
	vector<ModelInstance> m_objects;
};

