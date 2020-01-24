#pragma once
#include <vector>
#include "../Base/Object_Base.h"

class Triangle : public Object_Base {
private:
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec3> m_normals;

public:
	Triangle(
		std::vector<glm::vec3> vertices = { {0, 0, 0}, {0, 1, 1}, {1, 0, 1} },
		std::vector<glm::vec3> normals = { {0, 1, 0}, {0, 1, 0}, {0, 0, 1} },
		Colour colour = Colour(0, 0, 0, 0),
		bool reflexiveMesh = false,
		bool refractiveMesh = false)
		: m_vertices(vertices), m_normals(normals), Object_Base::Object_Base(colour, reflexiveMesh, refractiveMesh) {}
	~Triangle() {}

	virtual bool Intersection(Ray ray, HitInfo& hit) override;

	inline std::vector<glm::vec3> getVertices() const{ return this->m_vertices; }
	inline std::vector<glm::vec3> getNormals() const { return this->m_normals; }
};