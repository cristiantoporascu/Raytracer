#pragma once
#include "../Base/Object_Base.h"
#include "../Utils/BoundingBox.h"

class Sphere : public Object_Base {
private:
	float m_radius;
	glm::vec3 m_centre;

	BoundingBox m_bb;

public:
	Sphere(
		float radius = 5, 
		glm::vec3 centre = glm::vec3(0, 0, 0),
		Colour colour = Colour(0, 0, 0, 0),
		bool reflexiveMesh = false,
		bool refractiveMesh = false)
		: m_radius(radius), m_centre(centre), Object_Base::Object_Base(colour, reflexiveMesh, refractiveMesh) {};
	~Sphere() {};

	virtual bool Intersection(Ray ray, HitInfo& hit) override;	
	virtual BoundingBox getBB() const override;


	inline glm::vec3 getCentre() const { return this->m_centre; }
	inline float getRadius() const { return this->m_radius; }
};