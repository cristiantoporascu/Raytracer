#pragma once
#include "../Libraries/glm-0.9.6.3/glm/glm.hpp"
#include "../Utils/BoundingBox.h"
#include "../Primitives/Triangle.h"
#include "objloader/objloader.h"
#include <string>
#include <vector>

class Mesh {
private:
	glm::vec3 m_position;
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec2> m_uvs;
	std::vector<Triangle*> m_triangles;

	BoundingBox* m_bb;
	int octreeNodeNumber;

public:
	Mesh(Mesh& mesh) {
		this->m_position = mesh.getPosition();
		this->m_vertices = mesh.getVerticies();
		this->m_normals = mesh.getNormals();
		this->m_uvs = mesh.getUVs();
		this->m_triangles = mesh.getTriangles();
		this->m_bb = mesh.getBB();
		this->octreeNodeNumber = mesh.getNodeNumber();
	}

	Mesh(glm::vec3 pos, 
		std::vector<glm::vec3> vertices, 
		std::vector<glm::vec3> norm, 
		std::vector<glm::vec2> uvs, 
		std::vector<Triangle*> triangles,
		BoundingBox* box,
		int octreeNodeNumber)
		: m_position(pos), m_vertices(vertices), m_normals(norm), m_uvs(uvs), m_triangles(triangles), m_bb(box), octreeNodeNumber(octreeNodeNumber) {}

	~Mesh() {
		this->Clear();
	}

	static Mesh* importObjFile(const char* filePath, glm::vec3 pos, Colour colour, int octreeNodeNumber, bool reflexiveMesh = false, bool refractiveMesh = false) {
		std::vector< glm::vec3 > verticesImporter;
		std::vector< glm::vec3 > normalsImporter;
		std::vector< glm::vec2 > uvsImporter;
		std::vector< Triangle* > trianglesImporter;

		bool loadedTeaPot = loadOBJ(filePath, verticesImporter, normalsImporter);

		glm::vec3 min(0, 0, 0);
		glm::vec3 max(0, 0, 0);

		for (int i = 0; i < verticesImporter.size() / 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (verticesImporter[(i * 3) + j].x < min.x) min.x = verticesImporter[(i * 3) + j].x;
				if (verticesImporter[(i * 3) + j].y < min.y) min.y = verticesImporter[(i * 3) + j].y;
				if (verticesImporter[(i * 3) + j].z < min.z) min.z = verticesImporter[(i * 3) + j].z;

				if (verticesImporter[(i * 3) + j].x > max.x) max.x = verticesImporter[(i * 3) + j].x;
				if (verticesImporter[(i * 3) + j].y > max.y) max.y = verticesImporter[(i * 3) + j].y;
				if (verticesImporter[(i * 3) + j].z > max.z) max.z = verticesImporter[(i * 3) + j].z;
			}

			Triangle* newTriangle = new Triangle(
				std::vector<glm::vec3>{ 
					{ verticesImporter[(i * 3)] + pos },
					{ verticesImporter[(i * 3) + 1] + pos},
					{ verticesImporter[(i * 3) + 2] + pos}
				},
				std::vector<glm::vec3>{ 
					{ normalsImporter[(i * 3)] }, 
					{ normalsImporter[(i * 3) + 1] }, 
					{ normalsImporter[(i * 3) + 2] } },
				colour, reflexiveMesh, refractiveMesh);
			trianglesImporter.push_back(newTriangle);
		}

		BoundingBox* box = new BoundingBox((min + max) * 0.5f, max - min);
		
		Mesh* returnObject = new Mesh(
			pos,
			verticesImporter,
			normalsImporter,
			uvsImporter,
			trianglesImporter,
			box,
			octreeNodeNumber
		);

		returnObject->calculateOctreeBB(box);

		verticesImporter.clear();
		normalsImporter.clear();
		uvsImporter.clear();
		trianglesImporter.clear();

		return returnObject;
	}

	inline void Clear()
	{
		this->m_vertices.clear();
		this->m_normals.clear();
		this->m_uvs.clear();
		this->m_triangles.clear();
	}

	void calculateOctreeBB(BoundingBox* currentBox) {
		std::vector<Triangle*> triangles = this->m_triangles;
		glm::vec3 colour = triangles[0]->getColour().getColourVec3();

		float epsilon = 1e-2;

		std::vector<BoundingBox*> nodes;
		glm::vec3 tempColateralMin = glm::vec3(currentBox->getMin().x - epsilon, currentBox->getMin().y - epsilon, currentBox->getCentre().z - epsilon);
		glm::vec3 tempColateralMax = glm::vec3(currentBox->getCentre().x + epsilon, currentBox->getCentre().y + epsilon, currentBox->getMax().z + epsilon);
		nodes.push_back( new BoundingBox((tempColateralMin + tempColateralMax) * 0.5f, tempColateralMax - tempColateralMin));

		tempColateralMin = glm::vec3(currentBox->getMin().x - epsilon, currentBox->getCentre().y - epsilon, currentBox->getMin().z - epsilon);
		tempColateralMax = glm::vec3(currentBox->getCentre().x + epsilon, currentBox->getMax().y + epsilon, currentBox->getCentre().z + epsilon);
		nodes.push_back(new BoundingBox((tempColateralMin + tempColateralMax) * 0.5f, tempColateralMax - tempColateralMin));

		tempColateralMin = glm::vec3(currentBox->getCentre().x - epsilon, currentBox->getMin().y - epsilon, currentBox->getMin().z - epsilon);
		tempColateralMax = glm::vec3(currentBox->getMax().x + epsilon, currentBox->getCentre().y + epsilon, currentBox->getCentre().z + epsilon);
		nodes.push_back(new BoundingBox((tempColateralMin + tempColateralMax) * 0.5f, tempColateralMax - tempColateralMin));

		tempColateralMin = glm::vec3(currentBox->getCentre().x - epsilon, currentBox->getMin().y - epsilon, currentBox->getCentre().z - epsilon);
		tempColateralMax = glm::vec3(currentBox->getMax().x + epsilon, currentBox->getCentre().y + epsilon, currentBox->getMax().z + epsilon);
		nodes.push_back(new BoundingBox((tempColateralMin + tempColateralMax) * 0.5f, tempColateralMax - tempColateralMin));

		tempColateralMin = glm::vec3(currentBox->getCentre().x - epsilon, currentBox->getCentre().y - epsilon, currentBox->getMin().z - epsilon);
		tempColateralMax = glm::vec3(currentBox->getMax().x + epsilon, currentBox->getMax().y + epsilon, currentBox->getCentre().z + epsilon);
		nodes.push_back(new BoundingBox((tempColateralMin + tempColateralMax) * 0.5f, tempColateralMax - tempColateralMin));

		tempColateralMin = glm::vec3(currentBox->getMin().x - epsilon, currentBox->getCentre().y - epsilon, currentBox->getCentre().z - epsilon);
		tempColateralMax = glm::vec3(currentBox->getCentre().x + epsilon, currentBox->getMax().y + epsilon, currentBox->getMax().z + epsilon);
		nodes.push_back(new BoundingBox((tempColateralMin + tempColateralMax) * 0.5f, tempColateralMax - tempColateralMin));

		tempColateralMin = glm::vec3(currentBox->getMin().x - epsilon, currentBox->getMin().y - epsilon, currentBox->getMin().z - epsilon);
		tempColateralMax = glm::vec3(currentBox->getCentre().x + epsilon, currentBox->getMax().y + epsilon, currentBox->getCentre().z + epsilon);
		nodes.push_back(new BoundingBox((currentBox->getMin() + currentBox->getCentre()) * 0.5f, currentBox->getCentre() - currentBox->getMin()));

		tempColateralMin = glm::vec3(currentBox->getCentre().x - epsilon, currentBox->getCentre().y - epsilon, currentBox->getCentre().z - epsilon);
		tempColateralMax = glm::vec3(currentBox->getMax().x + epsilon, currentBox->getMax().y + epsilon, currentBox->getMax().z + epsilon);
		nodes.push_back(new BoundingBox((currentBox->getMax() + currentBox->getCentre()) * 0.5f, currentBox->getMax() - currentBox->getCentre()));

		for (int n = 0; n < nodes.size(); n++) {
			glm::vec3 min = nodes[n]->getMin();
			glm::vec3 max = nodes[n]->getMax();

			std::vector< Triangle* > triangleCalculator;
			for (int i = 0; i < triangles.size(); i++) {
				std::vector<glm::vec3> tempVertices = triangles[i]->getVertices();
				bool inBB = false;
				for (int j = 0; j < 3; j++) {
					glm::vec3 shifterInOrigin = tempVertices[j] - this->m_position;
					if (shifterInOrigin.x >= min.x &&
						shifterInOrigin.y >= min.y &&
						shifterInOrigin.z >= min.z &&
						shifterInOrigin.x <= max.x &&
						shifterInOrigin.y <= max.y &&
						shifterInOrigin.z <= max.z) {
						inBB = true;
					}
				}
				if (inBB) {
					triangleCalculator.push_back(triangles[i]);
				}

				if (triangleCalculator.size() <= 0 && i == triangles.size() - 1) {
					delete nodes[n];
					break;
				}
				
				if (triangleCalculator.size() > this->octreeNodeNumber) {
					triangleCalculator.clear();
					calculateOctreeBB(nodes[n]);
					currentBox->setOctree(nodes[n]);
					break;
				}

				if (i == triangles.size() - 1) {
					currentBox->setOctree(nodes[n]);
					nodes[n]->setOctreeData(triangleCalculator);
				}
			}
		}
	}

	inline const glm::vec3 getPosition() const { return this->m_position; }
	inline const std::vector<glm::vec3> getVerticies() const { return this->m_vertices; }
	inline const std::vector<glm::vec3> getNormals() const { return this->m_normals; }
	inline const std::vector<glm::vec2> getUVs() const { return this->m_uvs; }
	inline const std::vector<Triangle*> getTriangles() const { return this->m_triangles; }
	inline const int getNodeNumber() const { return this->octreeNodeNumber; }

	inline BoundingBox* getBB() { return this->m_bb; }
};