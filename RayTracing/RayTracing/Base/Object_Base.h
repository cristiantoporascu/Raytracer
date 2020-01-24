#pragma once
#include <glm-0.9.6.3/glm/glm.hpp>
#include "../Utils/Ray.h"
#include "../Utils/HitInfo.h"
#include "../Utils/BoundingBox.h"
#include "Colour.h"

class Object_Base {
protected:
	Colour colour = Colour(1.0f, 1.0f, 1.0f, 1.0f);
	bool reflexiveMesh;
	bool refractiveMesh;

public:
	const float EPSILON = 0.000001;

	Object_Base() {}
	Object_Base(const Colour& other, bool reflexiveMesh, bool refractiveMesh) { this->colour = other; this->reflexiveMesh = reflexiveMesh; this->refractiveMesh = refractiveMesh; }
	~Object_Base() {};

	virtual bool Intersection(Ray ray, HitInfo& hit) { return false; };
	virtual BoundingBox getBB() const { return BoundingBox(); }
	virtual BoundingBox getOctreeBB(BoundingBox* b) const { return *b; }

	inline Colour getColour() const { return this->colour; }
	inline bool isReflective() const { return this->reflexiveMesh; }
	inline bool isRefractive() const { return this->refractiveMesh; }
};