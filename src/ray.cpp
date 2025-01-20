#include "ray.h"

Ray::Ray()
{
}

Ray::Ray(const glm::dvec3& origin, const glm::dvec3& direction, double time)
	: m_origin(origin),
	  m_direction(direction),
	  m_time(time)
{
}

Ray::~Ray()
{
}

glm::dvec3 Ray::getOrigin() const
{
	return m_origin;
}

glm::dvec3 Ray::getDirection() const
{
	return m_direction;
}

double Ray::getTime() const
{
	return m_time;
}

glm::dvec3 Ray::at(double t) const
{
	return m_origin + t * m_direction;
}
