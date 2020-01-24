#include "Triangle.h"

bool Triangle::Intersection(Ray ray, HitInfo& hit) {
	glm::vec3 side1 = this->m_vertices[1] - this->m_vertices[0];
	glm::vec3 side2 = this->m_vertices[2] - this->m_vertices[0];

	glm::vec3 dirCross = glm::cross(ray.m_direction, side2);
	float dirDot = glm::dot(side1, dirCross);

	if (dirDot > -this->EPSILON && dirDot < this->EPSILON)
		return false;    // This ray is parallel to this triangle.

	glm::vec3 outerVector = ray.m_origin - this->m_vertices[0];

	float u = glm::dot(outerVector, dirCross) / dirDot;
	float v = glm::dot(ray.m_direction, glm::cross(outerVector, side1)) / dirDot;
	float w = 1 - u - v;

	if (u < 0 || u > 1) {
		return false;
	}
	else if (v < 0 || u + v > 1) {
		return false;
	}

	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = glm::dot(side2, glm::cross(outerVector, side1)) / dirDot;
	if (hit.distance == 0.0f || hit.distance > t) {
		hit.distance = t;
		hit.hitObject = this;
		hit.location = ray.m_direction * t;
		hit.normal = glm::normalize(w * this->m_normals[0] + u * this->m_normals[1] + v * this->m_normals[2]);
	}

	return true;
}