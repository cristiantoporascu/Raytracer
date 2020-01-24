#pragma once
#include "glm-0.9.6.3/glm/glm.hpp"
#include "../Base/Colour.h"
#include "HitInfo.h"
#include <vector>

class Light {
private:
	glm::vec3 m_position;
	glm::vec3 m_intensity;
	Colour m_colour;
	int area_size;
	int area_samples;
	std::vector<glm::vec3*> lightPoints;

public:
	Light(glm::vec3 pos, glm::vec3 intensity, Colour colour, int area_size = 0, int area_samples = 0)
		: m_position(pos), m_intensity(intensity), m_colour(colour), 
		area_size(area_size), area_samples(area_samples) {
		this->recalculateSamples(pos);
	}
	~Light() {}

	Colour CalculateLighting(
		std::vector<Object_Base*> sceneObjects,
		Ray ray,
		HitInfo& hit,
		int objectShininess,
		Colour ambientColour,
		Colour diffuseColour,
		Colour specularColour,
		bool shadows = false,
		bool randomiseShadows = false,
		bool reflectiveness = false,
		bool areaLight = false,
		int reflectDepth = 2,
		float IOR = 1.2f
	) {
		/* Ambient Light */
		Colour ambientLight = hit.hitObject->getColour() * ambientColour;

		/* Final Colour */
		Colour combColour(0, 0, 0, 0);

		glm::vec3 adjustedIntersection = hit.location + hit.normal * hit.hitObject->EPSILON;
		HitInfo hardLightHits;
		std::vector<HitInfo> softLightHits;

		std::vector<glm::vec3*> randomisedLights;

		// Plane only
		if (hit.hitObject == dynamic_cast<Plane*>(hit.hitObject)) {
			if (areaLight) {
				randomisedLights = (randomiseShadows ? this->randomisePos() : this->lightPoints);
				// Soft - Shadow
				for (unsigned int p = 0; p < randomisedLights.size(); p++) {
					HitInfo softLightHit;
					glm::vec3 lightRayDir = glm::normalize(*randomisedLights[p] - hit.location);
					Ray lightColisionRay(adjustedIntersection, lightRayDir);
					for (unsigned int i = 0; i < sceneObjects.size(); i++) {
						if (sceneObjects[i] != hit.hitObject) {
							sceneObjects[i]->Intersection(lightColisionRay, softLightHit);
						}
					}
					softLightHits.push_back(softLightHit);
				}
			}
			else {
				// Hard - Shadow
				glm::vec3 lightRayDir = glm::normalize(this->m_position - hit.location);
				Ray lightColisionRay(adjustedIntersection, lightRayDir);
				if (shadows) {
					for (unsigned int i = 0; i < sceneObjects.size(); i++) {
						if (sceneObjects[i] != hit.hitObject) {
							sceneObjects[i]->Intersection(lightColisionRay, hardLightHits);
						}
					}
				}
			}
		}
		

		if (areaLight && softLightHits.size() > 0) {
			for (unsigned int i = 0; i < softLightHits.size(); i++) {
				if (softLightHits[i].hitObject == nullptr || 
					softLightHits[i].distance > glm::length(*randomisedLights[i] - hit.location)) {
					/* Diffuse Light */
					Colour diffuseLight(diffuseColour.getColourVec3() * this->getIntensity() * glm::max(0.0f, glm::dot(glm::normalize(*randomisedLights[i] - hit.location), hit.normal)));

					/* Specular Light */
					Colour specularLight(
						specularColour.getColourVec3() * this->getIntensity() *
						std::pow(
							glm::max(0.0f, glm::dot(
								glm::reflect(glm::normalize(*randomisedLights[i] - hit.location), hit.normal), ray.m_direction)
							), objectShininess));

					combColour = combColour + (ambientLight + diffuseLight + specularLight).clamp();
				}
				else {
					combColour = combColour + ambientLight;
				}
			}
			combColour = (combColour / softLightHits.size()).clamp();
			
			if (randomiseShadows) {
				for (int i = 0; i < randomisedLights.size(); i++) {
					delete randomisedLights[i];
				}
				randomisedLights.clear();
			}
		}
		else {
			if (hardLightHits.hitObject == nullptr ||
				hardLightHits.distance > glm::length(this->m_position - hit.location)) {
				/* Diffuse Light */
				Colour diffuseLight(diffuseColour.getColourVec3() * this->m_intensity * glm::max(0.0f, glm::dot(glm::normalize(this->m_position - hit.location), hit.normal)));

				/* Specular Light */
				Colour specularLight(
					specularColour.getColourVec3() * this->m_intensity *
					std::pow(
						glm::max(0.0f, glm::dot(
							glm::reflect(glm::normalize(this->m_position - hit.location), hit.normal), ray.m_direction)
						), objectShininess));

				combColour = (ambientLight + diffuseLight + specularLight).clamp();
			}
			else {
				return ambientLight;
			}
		}

		if (!reflectiveness) {
			return combColour;
		}

		/* Refraction */
		if (hit.hitObject->isRefractive()) {
			Ray refractedRay(adjustedIntersection, glm::refract(ray.m_direction, hit.normal, 1.0f / 1.5f));
			HitInfo refractedHit;

			for (unsigned int i = 0; i < sceneObjects.size(); i++) {
				if (sceneObjects[i] != hit.hitObject) {
					sceneObjects[i]->Intersection(refractedRay, refractedHit);
				}
			}

			if (refractedHit.hitObject != nullptr) {
				Colour refractedColour =
					this->CalculateLighting(
						sceneObjects,
						refractedRay,
						refractedHit,
						objectShininess,
						ambientColour,
						diffuseColour,
						specularColour,
						shadows,
						randomiseShadows,
						reflectiveness,
						areaLight
					);

				combColour = refractedColour;
			}
			else {
				combColour = combColour + specularColour * Colour(168, 251, 255, 255);
			}
		}

		/* Reflectiveness */
		if (reflectDepth > 0 && hit.hitObject->isReflective()) {
			Ray reflectedRay(adjustedIntersection, glm::reflect(ray.m_direction, hit.normal));
			HitInfo reflectedHit;

			for (unsigned int i = 0; i < sceneObjects.size(); i++) {
				if (sceneObjects[i] != hit.hitObject) {
					sceneObjects[i]->Intersection(reflectedRay, reflectedHit);
				}
			}

			if (reflectedHit.hitObject != nullptr) {
				Colour reflectedColour =
					this->CalculateLighting(
						sceneObjects,
						reflectedRay,
						reflectedHit,
						objectShininess,
						ambientColour,
						diffuseColour,
						specularColour,
						shadows,
						randomiseShadows,
						reflectiveness,
						areaLight,
						reflectDepth - 1
					);

				return combColour + specularColour * reflectedColour;
			}
			else {
				return combColour;
			}
		}
		else {
			return combColour;
		}
	};

	inline glm::vec3 getPos() const { return this->m_position; }
	inline glm::vec3 getIntensity() const { return this->m_intensity; }
	inline Colour getColour() const { return this->m_colour; }
	inline int getSamplesNumber() const { return this->area_samples; }
	inline int getAreaSize() const { return this->area_size; }
	inline std::vector<glm::vec3*> getLightSamples() const { return this->lightPoints; }

	void setPos(glm::vec3 newPos) { this->m_position = newPos; recalculateSamples(newPos); }
	void setIntensity(glm::vec3 newIntensity) { this->m_intensity = newIntensity; }
	void setColour (Colour newColour) { this->m_colour = newColour; }
	void setSamplesNumber(int newNumberSamples) { this->area_samples = newNumberSamples; recalculateSamples(this->m_position); }
	void setAreaSize(int newSize) { this->area_size = newSize; }

	void recalculateSamples(glm::vec3 newPos) {
		if (this->area_size && this->area_samples) {
			for (int i = 0; i < this->lightPoints.size(); i++) {
				delete this->lightPoints[i];
			}
			this->lightPoints.clear();
			int number = floor(sqrt(this->area_samples));
			float cellSize = this->area_size / number;
			float startingPosX = newPos.x - this->area_size / 2;
			float startingPosZ = newPos.z - this->area_size / 2;
			for (float i = 0; i < number; i++) {
				for (float j = 0; j < number; j++) {
					lightPoints.push_back(
						new glm::vec3(startingPosX + i * cellSize, 
							newPos.y, 
							startingPosZ + j * cellSize));
				}
			}
		}
	}

	std::vector<glm::vec3*> randomisePos() {
		if (this->area_size && this->area_samples) {
			std::vector<glm::vec3*> tempLights;
			int number = floor(sqrt(this->area_samples));
			float cellSize = this->area_size / number;
			for (int i = 0; i < this->lightPoints.size(); i++) {
				tempLights.push_back(
					new glm::vec3(lightPoints[i]->x
						+ static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * cellSize,
						lightPoints[i]->y,
						lightPoints[i]->z
					+ static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* cellSize));
			}
			return tempLights;
		}
	}
};