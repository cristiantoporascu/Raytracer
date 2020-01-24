#pragma once
#include "glm-0.9.6.3/glm/glm.hpp"

class Colour {
protected:
	glm::vec4 m_colour;

public:
	Colour(float r = 0, float g = 0, float b = 0, float a = 1.0f) { this->m_colour = glm::vec4(r, g, b, a); }
	Colour(int r = 0, int g = 0, int b = 0, int a = 255) { this->m_colour = glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f); }
	Colour(const glm::vec3& vec) { this->m_colour = glm::vec4(vec.x, vec.y, vec.z, 1.0); }
	Colour(const glm::vec4& vec) { this->m_colour = vec; }
	Colour(const Colour& other) { this->m_colour = other.m_colour; }

	inline glm::vec4 getColour() const { return this->m_colour; }
	inline glm::vec3 getColourVec3() const { return glm::vec3(this->m_colour.x, this->m_colour.y, this->m_colour.z); }

	inline glm::tvec4<uint8_t> ToRGBA() const {
		return glm::tvec4<uint8_t>(
			255 * glm::clamp(0.0f, 1.0f, this->m_colour.x),
			255 * glm::clamp(0.0f, 1.0f, this->m_colour.y),
			255 * glm::clamp(0.0f, 1.0f, this->m_colour.z),
			255 * glm::clamp(0.0f, 1.0f, this->m_colour.w)
		);
	}

	inline glm::tvec3<uint8_t> ToRGB() const {
		return glm::tvec3<uint8_t>(
			255 * glm::clamp(0.0f, 1.0f, this->m_colour.x),
			255 * glm::clamp(0.0f, 1.0f, this->m_colour.y),
			255 * glm::clamp(0.0f, 1.0f, this->m_colour.z)
			);
	}

	inline Colour clamp() const {
		return Colour(
			glm::clamp(0.0f, 1.0f, this->m_colour.x),
			glm::clamp(0.0f, 1.0f, this->m_colour.y),
			glm::clamp(0.0f, 1.0f, this->m_colour.z),
			glm::clamp(0.0f, 1.0f, this->m_colour.w)
		);
	}

	Colour operator+(const Colour& other) {
		return Colour(
			glm::vec4(
				this->m_colour.x + other.getColour().x,
				this->m_colour.y + other.getColour().y,
				this->m_colour.z + other.getColour().z,
				this->m_colour.w + other.getColour().w
			)
		);
	}

	Colour operator+=(const Colour& other) {
		return Colour(
			glm::vec4(
				this->m_colour.x + other.getColour().x,
				this->m_colour.y + other.getColour().y,
				this->m_colour.z + other.getColour().z,
				this->m_colour.w + other.getColour().w
			)
		);
	}

	Colour operator*(const Colour& other) {
		return Colour(
			glm::vec4(
				this->m_colour.x * other.getColour().x,
				this->m_colour.y * other.getColour().y,
				this->m_colour.z * other.getColour().z,
				this->m_colour.w * other.getColour().w
			)
		);
	}

	Colour operator/(const int other) {
		return Colour(
			glm::vec4(
				this->m_colour.x / other,
				this->m_colour.y / other,
				this->m_colour.z / other,
				this->m_colour.w / other
			)
		);
	}

	Colour operator/=(const int other) {
		return Colour(
			glm::vec4(
				this->m_colour.x / other,
				this->m_colour.y / other,
				this->m_colour.z / other,
				this->m_colour.w / other
			)
		);
	}
};