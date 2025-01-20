#include "sphere.h"

#include <qdebug.h>

Sphere::Sphere(glm::dvec3 center, double r, Material::Ptr material)
	: m_center(center),
	  m_r(r),
      m_material(material)
{
    glm::dvec3 rVec = glm::dvec3(m_r, m_r, m_r);
    m_aabb = Aabb(m_center - rVec, m_center + rVec);
}

Sphere::Sphere(const glm::dvec3& center, double r, const glm::dvec3& speed, const Material::Ptr& material)
    : m_center(center),
      m_r(r),
      m_speed(speed),
      m_material(material)
{
    glm::dvec3 rVec = glm::dvec3(m_r, m_r, m_r);
    glm::dvec3 center0 = m_center;
    glm::dvec3 center1 = m_center + m_speed;
    Aabb aabb0 = Aabb(center0 - rVec, center0 + rVec);
    Aabb aabb1 = Aabb(center1 - rVec, center1 + rVec);
    m_aabb = Aabb(aabb0, aabb1);
}

Sphere::~Sphere()
{
}

glm::dvec3 Sphere::position(double time) const
{
    return m_center + m_speed * time;
}

bool Sphere::hit(const Ray& ray, Interval rayT, HitRecord& rec) const
{
    glm::dvec3 pos = position(ray.getTime());

    glm::dvec3 oc = pos - ray.getOrigin();;
    double a = glm::dot(ray.getDirection(), ray.getDirection());
    double b = -2.0 * glm::dot(ray.getDirection(), oc);
    double c = glm::dot(oc, oc) - m_r * m_r;
    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
    {
        return false;
    }

    // 计算两个 t 值
    double sqrtDiscriminant = sqrt(discriminant);
    double t1 = (-b - sqrtDiscriminant) / (2.0 * a);
    double t2 = (-b + sqrtDiscriminant) / (2.0 * a);

    // 找到第一个有效的交点
    if (t1 < rayT.getMin() || t1 > rayT.getMax())
    {
        t1 = t2; // 尝试第二个交点
        if (t1 < rayT.getMin() || t1 > rayT.getMax())
        {
            return false; // 两个交点都不在范围内
        }
    }

    rec.t = t1;
    rec.pos = ray.at(t1);
    
    //法线始终与光线方向相反
    glm::dvec3 normal = glm::normalize(rec.pos - pos);
    rec.frontFace = glm::dot(ray.getDirection(), normal) < 0;
    if (rec.frontFace)
        rec.normal = normal;
    else
        rec.normal = -normal;
    rec.material = m_material;
    getSphereUV(normal, rec.u, rec.v);

    return true;
}

void Sphere::getSphereUV(const glm::dvec3& pos, double& u, double& v)
{
    auto theta = std::acos(-pos.y);
    auto phi = std::atan2(-pos.z, pos.x) + Pi;

    u = phi / (2 * Pi);
    v = theta / Pi;
}
