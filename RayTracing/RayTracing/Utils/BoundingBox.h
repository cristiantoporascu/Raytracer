#pragma once
#include "glm-0.9.6.3/glm/glm.hpp"
#include "Ray.h"
#include <vector>

/* Forward declaration */
class Triangle;

class BoundingBox {
private:
	glm::vec3 m_centre;
	glm::vec3 m_size;
	glm::vec3 m_min;
	glm::vec3 m_max;

	std::vector<BoundingBox*> m_octree;
	std::vector<Triangle*> m_octreeData;

public:
	BoundingBox(glm::vec3 centre = glm::vec3(0, 0, 0), glm::vec3 size = glm::vec3(1, 1, 1));
	~BoundingBox();

	inline void setOctree(BoundingBox* octreeBB) { this->m_octree.push_back(octreeBB); }
	inline void setOctreeData(std::vector<Triangle*> octreeDataSet) { this->m_octreeData = octreeDataSet; }

	void setCentreSize(glm::vec3 newCentre, glm::vec3 newSize);
	void setMinMax(glm::vec3 newMin, glm::vec3 newMax);

	bool Intersection(Ray ray) const;
	bool Intersection(const BoundingBox& other) const;

	inline glm::vec3 getCentre() const { return this->m_centre; }
	inline glm::vec3 getSize() const { return this->m_size; }

	inline glm::vec3 getMin() const { return this->m_min; }
	inline glm::vec3 getMax() const { return this->m_max; }

	inline const std::vector<BoundingBox*> getOctree() { return this->m_octree; }
	inline const std::vector<Triangle*> getOctreeData() { return this->m_octreeData; }
};