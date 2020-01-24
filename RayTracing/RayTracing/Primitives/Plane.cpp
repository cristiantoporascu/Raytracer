#include "Plane.h"

bool Plane::Intersection(Ray ray, HitInfo& hit) {
	float t = (glm::dot((this->m_position - ray.m_origin), this->m_normal)) / glm::dot(ray.m_direction, this->m_normal);

	if (t > -this->EPSILON && t < this->EPSILON)
		return false;

	if (t > 0) {
		if (hit.distance == 0.0f || hit.distance > t) {
			hit.distance = t;
			hit.hitObject = this;
			hit.normal = this->m_normal;
			hit.location = ray.m_origin + ray.m_direction * (float)t;
		}

		return true;
	}
	else {
		return false;
	}
}