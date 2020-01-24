#include "Object.h"

BoundingBox Object::getBB() const
{
	if (this->m_mesh == nullptr)
		return BoundingBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));

	BoundingBox box = (*this->m_mesh->getBB());
	box.setCentreSize(box.getCentre() + this->m_mesh->getPosition(), box.getSize());
	return box;
}

BoundingBox Object::getOctreeBB(BoundingBox* b) const
{
	return BoundingBox(b->getCentre() + this->m_mesh->getPosition(), b->getSize());
}

bool Object::Intersection(Ray ray, HitInfo& hit) {
	if (m_mesh == nullptr)
		return false;

	if (!getBB().Intersection(ray))
		return false;

	octreeIntersection(ray, hit, this->m_mesh->getBB());

	//std::vector <Triangle*> tempMesh = this->m_mesh->getTriangles();

	//for (int i = 0; i < tempMesh.size(); i++) {
	//	tempMesh[i]->Intersection(ray, hit);
	//}

	if (hit.hitObject != nullptr) {
		return true;
	}

	return false;
}

void Object::octreeIntersection(Ray ray, HitInfo& hit, BoundingBox* box) {
	std::vector<BoundingBox*> octree = box->getOctree();

	if (octree.size() > 0) {
		for (int i = 0; i < octree.size(); i++) {
			if (getOctreeBB(octree[i]).Intersection(ray)) {
				octreeIntersection(ray, hit, octree[i]);
			}
		}
	}
	else {
		std::vector <Triangle*> tempMesh = box->getOctreeData();

		for (int i = 0; i < tempMesh.size(); i++) {
			tempMesh[i]->Intersection(ray, hit);
		}
	}
}