//------------------------------------------------------------------------------
// A basic scene class and description
//
//------------------------------------------------------------------------------
#pragma once

#include "RayTrace.h"
#include <memory>

class Shape;

struct Scene {
	glm::vec3 lightPosition;
	glm::vec3 lightColor;
	float ambientFactor;
	std::vector<std::shared_ptr<Shape>> shapesInScene;
};


Scene initScene1();
Scene initScene2();

