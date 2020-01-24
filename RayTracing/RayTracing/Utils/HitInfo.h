#pragma once
#include "../Libraries/glm-0.9.6.3/glm/glm.hpp"
#include "../Base/Object_Base.h"

class HitInfo
{
public:
	class Object_Base* hitObject = nullptr;
	float distance = 0.0f;

	glm::vec3 location;
	glm::vec3 normal;
	glm::vec2 uvs;
};