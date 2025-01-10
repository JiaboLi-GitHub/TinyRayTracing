#include "sphere.h"

#include <qdebug.h>

Sphere::Sphere(glm::dvec3 center, double r, Material::Ptr material)
	: m_center(center),
	  m_r(r),
      m_material(material)
{
}

Sphere::~Sphere()
{
}

bool Sphere::hit(const Ray& ray, Interval rayT, HitRecord& rec) const
{
    glm::dvec3 oc = m_center - ray.getOrigin();;
    double a = glm::dot(ray.getDirection(), ray.getDirection());
    double b = -2.0 * glm::dot(ray.getDirection(), oc);
    double c = glm::dot(oc, oc) - m_r * m_r;
    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
    {
        return false;
    }
    else
    {
        //qDebug() << "hit true";
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
    glm::dvec3 normal = glm::normalize(rec.pos - m_center);
    rec.frontFace = glm::dot(ray.getDirection(), normal) < 0;
    if (rec.frontFace)
        rec.normal = normal;
    else
        rec.normal = -normal;
    rec.material = m_material;

    return true;
}
