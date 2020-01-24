#pragma once
#include "../Libraries/glm-0.9.6.3/glm/glm.hpp"

class Camera {
private:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	int m_FOV;

public:
	Camera(glm::vec3 initialPos, glm::vec3 initialRot, int defaultFOV = 90)
		: m_position(initialPos), m_rotation(initialRot), m_FOV(defaultFOV) {};
	~Camera() {};

	inline glm::vec3 getPos() const { return this->m_position; }
	inline glm::vec3 getRot() const { return this->m_rotation; }
	inline int getFOV() const { return this->m_FOV; }

	inline void setPos(glm::vec3 newPos) { this->m_position = newPos; }
	inline void setRot(glm::vec3 newRot) { this->m_rotation = newRot; }
	inline void setFOV(int newFOV) { this->m_FOV = newFOV; }

	glm::vec3 getForwardVector() const;
	glm::vec3 getRightVector() const;
};