#include "BoundingBox.h"

BoundingBox::BoundingBox(glm::vec3 centre, glm::vec3 size)
	: m_centre(centre), m_size(size) {
	this->m_min = centre - size * 0.5f;
	this->m_max = centre + size * 0.5f;
}

BoundingBox::~BoundingBox() {
	this->m_octree.clear();

	for (int i = 0; i < this->m_octreeData.size(); i++) {
		delete this->m_octreeData[i];
	}
}

void BoundingBox::setCentreSize(glm::vec3 newCentre, glm::vec3 newSize)
{
	this->m_centre = newCentre;
	this->m_size = newSize;
	this->setMinMax(newCentre - newSize * 0.5f, newCentre + newSize * 0.5f);
}

void BoundingBox::setMinMax(glm::vec3 newMin, glm::vec3 newMax)
{
	this->m_min = newMin;
	this->m_max = newMax;
}

bool BoundingBox::Intersection(Ray ray) const
{
	float invDirX = 1 / ray.m_direction.x;
	float invDirY = 1 / ray.m_direction.y;
	float invDirZ = 1 / ray.m_direction.z;

	float tmin = (this->m_min.x - ray.m_origin.x) * invDirX;
	float tmax = (this->m_max.x - ray.m_origin.x) * invDirX;

	if (tmin > tmax) {
		float temp = tmin;
		tmin = tmax;
		tmax = temp;
	}

	float tymin = (this->m_min.y - ray.m_origin.y) * invDirY;
	float tymax = (this->m_max.y - ray.m_origin.y) * invDirY;

	if (tymin > tymax) {
		float temp = tymin;
		tymin = tymax;
		tymax = temp;
	}

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (this->m_min.z - ray.m_origin.z) * invDirZ;
	float tzmax = (this->m_max.z - ray.m_origin.z) * invDirZ;

	if (tzmin > tzmax) {
		float temp = tzmin;
		tzmin = tzmax;
		tzmax = temp;
	}

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	return true;
}

bool BoundingBox::Intersection(const BoundingBox& other) const
{
	const glm::vec3 otherMin = other.getMin();
	const glm::vec3 otherMax = other.getMax();

	return !(
		otherMax.x < this->m_min.x || otherMax.y < this->m_min.y || otherMax.z < this->m_min.z ||
		otherMin.x > this->m_max.x || otherMin.y > this->m_max.y || otherMin.z > this->m_max.z
		);
}