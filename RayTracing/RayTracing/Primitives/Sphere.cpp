#include "Sphere.h"

BoundingBox Sphere::getBB() const
{
	BoundingBox box(this->m_centre, glm::vec3(1, 1, 1) * this->m_radius * 2.0f);
	return box;
}

bool Sphere::Intersection(Ray ray, HitInfo& hit) {
	if (!getBB().Intersection(ray))
		return false;

	glm::vec3 originToSphere = this->m_centre - ray.m_origin;
	float tca = glm::dot(originToSphere, ray.m_direction);
	if (tca < 0) return false;

	float radiusSquared = this->m_radius * this->m_radius;
	float tcaSquared = tca * tca;

	float projectOTSonTCA = glm::dot(originToSphere, originToSphere) - tcaSquared;
	if (projectOTSonTCA > radiusSquared) return false;

	float thc = std::sqrt(radiusSquared - projectOTSonTCA);
	float t0 = tca - thc;
	float t1 = tca + thc;

	if (hit.distance == 0.0f || hit.distance > tca) {
		hit.distance = tca;
		hit.hitObject = this;
		hit.location = ray.m_direction * t0;
		hit.normal = glm::normalize(hit.location - originToSphere);
	}

	return true;
}