#ifndef RAY_CUH
#define RAY_CUH
#include "global.cuh"

class Ray
{
public:
    __device__ Ray();

    __device__ Ray(const vec3& origin,const vec3& direction);

    __device__ vec3 getOrigin() const;

    __device__ vec3 getDirection() const;

    __device__ vec3 at(double t) const;

private:
    vec3 m_origin;
    vec3 m_direction;
};

#endif //RAY_CUH
