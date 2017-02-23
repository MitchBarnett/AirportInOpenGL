#pragma once
#include <vector>
#include "Model.h"
class Scene
{
public:
	Scene();
	void load();
	void update();
	void draw();
private:
	std::vector<Model*> m_models;
};

