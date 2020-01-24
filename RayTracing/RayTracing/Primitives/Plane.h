#pragma once
#include "../Base/Object_Base.h"

class Plane : public Object_Base {
private:
	glm::vec3 m_position;
	glm::vec3 m_normal;
public:
	Plane(
		glm::vec3 pos = glm::vec3(0, 0, 0), 
		glm::vec3 normal = glm::vec3(0, 1, 0), 
		Colour colour = Colour(0, 0, 0, 0),
		bool reflexiveMesh = false,
		bool refractiveMesh = false)
		: m_position(pos), m_normal(normal), Object_Base::Object_Base(colour, reflexiveMesh, refractiveMesh) {};
	~Plane() {};

	virtual bool Intersection(Ray ray, HitInfo& hit) override;

	inline glm::vec3 getPos() const { return this->m_position; }
	inline glm::vec3 getNormal() const { return this->m_normal; }
};