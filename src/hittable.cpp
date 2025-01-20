#include "hittable.h"
#include "quad.h"

Aabb Hittable::getAabb()
{
    return m_aabb;
}

Translate::Translate(const Hittable::Ptr& hittable, const glm::dvec3& offset)
    : m_hittable(hittable),
      m_offset(offset)
{
    m_aabb = hittable->getAabb() + offset;
}

bool Translate::hit(const Ray& ray, Interval rayT, HitRecord& rec) const
{
    Ray offset_r(ray.getOrigin() - m_offset, ray.getDirection(), ray.getTime());

    if (!m_hittable->hit(offset_r, rayT, rec))
        return false;

    rec.pos += m_offset;

    return true;
}



RotateY::RotateY(const Hittable::Ptr& hittable, double angle)
    : m_hittable(hittable)
{
    auto radians = toRadians(angle);
    m_sinTheta = std::sin(radians);
    m_cosTheta = std::cos(radians);
    m_aabb = m_hittable->getAabb();

    glm::dvec3 min(Infinity, Infinity, Infinity);
    glm::dvec3 max(-Infinity, -Infinity, -Infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                auto x = i * m_aabb.getX().getMax() + (1 - i) * m_aabb.getX().getMin();
                auto y = j * m_aabb.getY().getMax() + (1 - j) * m_aabb.getY().getMin();
                auto z = k * m_aabb.getZ().getMax() + (1 - k) * m_aabb.getZ().getMin();

                auto newx = m_cosTheta * x + m_sinTheta * z;
                auto newz = -m_sinTheta * x + m_cosTheta * z;

                glm::dvec3 tester(newx, y, newz);

                for (int c = 0; c < 3; c++) {
                    min[c] = std::fmin(min[c], tester[c]);
                    max[c] = std::fmax(max[c], tester[c]);
                }
            }
        }
    }

    m_aabb = Aabb(min, max);
}

bool RotateY::hit(const Ray& ray, Interval rayT, HitRecord& rec) const
{
    auto origin = glm::dvec3(
        (m_cosTheta * ray.getOrigin().x) - (m_sinTheta * ray.getOrigin().z),
        ray.getOrigin().y,
        (m_sinTheta * ray.getOrigin().x) + (m_cosTheta * ray.getOrigin().z)
    );

    auto direction = glm::dvec3(
        (m_cosTheta * ray.getDirection().x) - (m_sinTheta * ray.getDirection().z),
        ray.getDirection().y,
        (m_sinTheta * ray.getDirection().x) + (m_cosTheta * ray.getDirection().z)
    );

    Ray rotated_r(origin, direction, ray.getTime());

    if (!m_hittable->hit(rotated_r, rayT, rec))
        return false;

    rec.pos = glm::dvec3(
        (m_cosTheta * rec.pos.x) + (m_sinTheta * rec.pos.z),
        rec.pos.y,
        (-m_sinTheta * rec.pos.x) + (m_cosTheta * rec.pos.z)
    );

    rec.normal = glm::dvec3(
        (m_cosTheta * rec.normal.x) + (m_sinTheta * rec.normal.z),
        rec.normal.y,
        (-m_sinTheta * rec.normal.x) + (m_cosTheta * rec.normal.z)
    );

    return true;
}
