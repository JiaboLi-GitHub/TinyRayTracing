#include "ray.cuh"

__device__ Ray::Ray()
    : m_origin(vec3(0,0,0)), m_direction(vec3(0,0,0))
{

}

__device__ Ray::Ray(const vec3& origin,const vec3& direction)
    : m_origin(origin), m_direction(direction)
{

}

__device__ vec3 Ray::getOrigin() const
{
    return m_origin;
}

__device__ vec3 Ray::getDirection() const
{
    return m_direction;
}

__device__ vec3 Ray::at(double t) const
{
    return m_origin + t * m_direction;
}
