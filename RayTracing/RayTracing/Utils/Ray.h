#pragma once
#include "glm-0.9.6.3/glm/glm.hpp"

class Ray
{
public:
	glm::vec3 m_origin;
	glm::vec3 m_direction;

	Ray(
		glm::vec3 origin,
		glm::vec3 direction)
		: m_origin(origin), m_direction(glm::normalize(direction)) {}

};
