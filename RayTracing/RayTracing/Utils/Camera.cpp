#include "Camera.h"

glm::vec3 Camera::getForwardVector() const {
	glm::vec3 rot = glm::radians(this->m_rotation);

	return glm::normalize(glm::vec3(
		glm::cos(rot.x) * glm::sin(rot.y),
		-glm::sin(rot.x),
		glm::cos(rot.x) * glm::cos(rot.y)
	));
}

glm::vec3 Camera::getRightVector() const {
	glm::vec3 rot = glm::radians(this->m_rotation);

	return glm::normalize(glm::vec3(
		cos(rot.z) * cos(rot.y),
		sin(rot.z),
		cos(rot.z) * -sin(rot.y)
	));
}