#include "hittable.cuh"

__device__ Hittable::Hittable()
{
}

__device__ Hittable::~Hittable()
{
}

__device__ HittableList::HittableList(Hittable** hittables, size_t size)
    : m_hittables(hittables), m_size(size)
{
}

__device__ bool HittableList::hit(const Ray& ray, Interval rayT, HitRecord& rec) const
{
    HitRecord tmpRec;
    bool hitAnything = false;
    double closestSoFar = rayT.getMax();

    for (int i = 0; i < m_size; ++i)
    {
        Hittable* hittable = m_hittables[i];
        if (hittable->hit(ray, Interval(rayT.getMin(), closestSoFar), tmpRec))
        {
            hitAnything = true;
            closestSoFar = tmpRec.t;
            rec = tmpRec;
        }
    }

    return hitAnything;
}

__device__ Sphere::Sphere(const vec3& center, double r, Material* material)
    : m_center(center), m_r(r), m_material(material)
{
}

__device__ Sphere::~Sphere()
{
}

__device__ bool Sphere::hit(const Ray& ray, Interval rayT, HitRecord& rec) const
{
    vec3 oc = m_center - ray.getOrigin();
    double a = dot(ray.getDirection(), ray.getDirection());
    double b = -2.0 * dot(ray.getDirection(), oc);
    double c = dot(oc, oc) - m_r * m_r;
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
    vec3 normal = unit_vector(rec.pos - m_center);
    rec.frontFace = dot(ray.getDirection(), normal) < 0;
    if (rec.frontFace)
        rec.normal = normal;
    else
        rec.normal = -normal;
    rec.material = m_material;

    return true;
}
