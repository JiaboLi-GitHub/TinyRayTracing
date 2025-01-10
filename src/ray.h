#pragma once

#include <glm/glm.hpp>

class Ray
{
public:
	Ray();

	Ray(const glm::dvec3& origin, const glm::dvec3& direction);

	~Ray();

	glm::dvec3 getOrigin() const;

	glm::dvec3 getDirection() const;

	glm::dvec3 at(double t) const;

private:
	glm::dvec3 m_origin = glm::dvec3(0.0);
	glm::dvec3 m_direction = glm::dvec3(0.0);
};

