#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

extern int CanvasWidth;
extern double AspectRatio;

enum class SceneType
{
    OneWeekend,     // 周末
    NextWeek,       // 下周
    CornellBox      // 康奈尔盒
};
extern SceneType CurrentSceneType;

const double Infinity = std::numeric_limits<double>::infinity();	//无穷大
const double Pi = 3.1415926535897932385;

inline double toRadians(double degrees) 
{
    return degrees * Pi / 180.0;
}

// 返回[0,1)区间的实数
inline double randomDouble() 
{
    return std::rand() / (RAND_MAX + 1.0);
}

// 返回[min,max)区间的实数
inline double randomDouble(double min, double max)
{
    return min + (max - min) * randomDouble();
}

inline glm::dvec3 randomVec3()
{
    return glm::dvec3(randomDouble(), randomDouble(), randomDouble());
}

inline glm::dvec3 randomVec3(double min, double max)
{
    return glm::dvec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
}

inline glm::dvec3 randomNormalizeVec3()
{
    while (true)
    {
        auto vec3 = randomVec3(-1, 1);
        if (!(vec3.x == 0 && vec3.y == 0 && vec3.z == 0))
        {
            return glm::normalize(vec3);
        }
    }
}

inline glm::dvec3 randomHemisphere(const glm::dvec3& normal)
{
    glm::dvec3 vec3 = randomNormalizeVec3();
    if (glm::dot(vec3, normal) > 0.0)
    { 
        return vec3;
    }
    else 
    {
        return -vec3;
    }
}

// 片段是否为0向量
inline bool nearZero(glm::dvec3 vec3)
{
    double s = 1e-8;
    return (std::fabs(vec3.x) < s) && (std::fabs(vec3.y) < s) && (std::fabs(vec3.z) < s);
}

// 镜面反射
inline glm::dvec3 reflect(const glm::dvec3& v, const glm::dvec3& normal)
{
    return v - 2 * dot(v, normal) * normal;
}

// 介质折射
inline glm::dvec3 refract(const glm::dvec3& v, const glm::dvec3& n, double ratio) 
{
    auto cos_theta = std::fmin(glm::dot(-v, n), 1.0);
    glm::dvec3 r_out_perp = ratio * (v + cos_theta * n);
    glm::dvec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - glm::dot(r_out_perp, r_out_perp))) * n;
    return r_out_perp + r_out_parallel;
}

inline glm::dvec3 randomDisk()
{
    while (true)
    {
        glm::dvec3 vec3 = glm::dvec3(randomDouble(-1, 1), randomDouble(-1, 1), 0);
        if (!(vec3.x == 0 && vec3.y == 0 && vec3.z == 0))
        {
            return glm::normalize(vec3);
        }
    }
}
