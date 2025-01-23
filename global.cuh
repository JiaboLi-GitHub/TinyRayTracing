#ifndef GLOBAL_CUH
#define GLOBAL_CUH

#include <iostream>
#include <vector>
#include <memory.h>

#include <curand_kernel.h>

#include "vec3.cuh"

extern double AspectRatio;
extern int ImageWidth;
extern int ImageHeight;
extern int ImageSize;
extern const dim3 BlockSize;
extern const dim3 GridSize;

// 设备变量
__device__ extern double AspectRatioD;
__device__ extern int ImageWidthD;
__device__ extern int ImageHeightD;
__device__ extern int ImageSizeD;
__device__ const double Pi = 3.1415926535897932385;
__device__ const int RAND_MAX_D = 2147483647;
__device__ const double InfinityD = 1.0e308;	//无穷大
__device__ extern curandState* RandStateD;

__device__ static double toRadians(double degrees)
{
    return degrees * Pi / 180.0;
}

// 返回[0,1)区间的实数
__device__ static double randomDouble()
{
    int x = threadIdx.x + blockIdx.x * blockDim.x;
    int y = threadIdx.y + blockIdx.y * blockDim.y;
    int index = x + y * ImageWidthD;
    return curand_uniform(&RandStateD[index]);
}

__device__ static double randomDouble(double min, double max)
{
    return min + (max - min) * randomDouble();
}

__device__ static vec3 randomDisk()
{
    while (true)
    {
        vec3 v = vec3(randomDouble(-1, 1), randomDouble(-1, 1), 0);
        if (!(v.x() == 0 && v.y() == 0 && v.z() == 0))
        {
            return unit_vector(v);
        }
    }
}

__device__ static vec3 randomVec3()
{
    return vec3(randomDouble(), randomDouble(), randomDouble());
}

__device__ static vec3 randomVec3(double min, double max)
{
    return vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
}

__device__ static vec3 randomNormalizeVec3()
{
    while (true)
    {
        auto v = randomVec3(-1, 1);
        if (!(v.x() == 0 && v.y() == 0 && v.z() == 0))
        {
            return unit_vector(v);
        }
    }
}

// 片段是否为0向量
__device__ static bool nearZero(vec3 v)
{
    double s = 1e-8;
    return (fabs(v.x()) < s) && (fabs(v.y()) < s) && (fabs(v.z()) < s);
}

// 镜面反射
__device__ static vec3 reflect(const vec3& v, const vec3& normal)
{
    return v - 2 * dot(v, normal) * normal;
}

// 介质折射
__device__ static vec3 refract(const vec3& v, const vec3& n, double ratio)
{
    auto cos_theta = std::fmin(dot(-v, n), 1.0);
    vec3 r_out_perp = ratio * (v + cos_theta * n);
    vec3 r_out_parallel = -sqrt(std::fabs(1.0 - dot(r_out_perp, r_out_perp))) * n;
    return r_out_perp + r_out_parallel;
}

#endif //GLOBAL_CUH
