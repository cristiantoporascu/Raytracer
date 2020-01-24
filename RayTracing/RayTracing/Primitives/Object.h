#pragma once
#include "../Base/Object_Base.h"
#include "../Base/Mesh.h"

class Object : public Object_Base {
private:
	Mesh* m_mesh = nullptr;

public:
	Object(Mesh* mesh) : m_mesh(mesh) {}
	~Object() {}

	void octreeIntersection(Ray ray, HitInfo& hit, BoundingBox* currentBox);
	virtual bool Intersection(Ray ray, HitInfo& hit) override;
	virtual BoundingBox getBB() const override;
	virtual BoundingBox getOctreeBB(BoundingBox* b) const override;

	inline Mesh* getMesh() const { return this->m_mesh; }

};