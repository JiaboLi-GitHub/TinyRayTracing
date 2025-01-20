#include "quad.h"

Quad::Quad(glm::dvec3 q, glm::dvec3 u, glm::dvec3 v, Material::Ptr material)
	: m_q(q),
	  m_u(u),
	  m_v(v),
	  m_material(material)
{
	glm::dvec3 n = glm::cross(u, v);
    m_normal = glm::normalize(n);
	m_d = glm::dot(m_normal, m_q);
	m_w = n / glm::dot(n, n);

	setBoundingBox();
}

void Quad::setBoundingBox()
{
	auto aabb1 = Aabb(m_q, m_q + m_u + m_v);
	auto aabb2 = Aabb(m_q + m_u, m_q + m_v);
	m_aabb = Aabb(aabb1, aabb2);
}

bool Quad::hit(const Ray& ray, Interval rayT, HitRecord& rec) const
{
    auto denom = glm::dot(m_normal, ray.getDirection());

    if (std::fabs(denom) < 1e-8)
        return false;

    auto t = (m_d - glm::dot(m_normal, ray.getOrigin())) / denom;
    if (!rayT.contains(t))
        return false;

    auto intersection = ray.at(t);
    glm::dvec3 planar_hitpt_vector = intersection - m_q;
    auto alpha = glm::dot(m_w, glm::cross(planar_hitpt_vector, m_v));
    auto beta = glm::dot(m_w, glm::cross(m_u, planar_hitpt_vector));

    if (!isInterior(alpha, beta, rec))
        return false;

    rec.t = t;
    rec.pos = intersection;
    rec.material = m_material;

    //法线始终与光线方向相反
    //glm::dvec3 normal = glm::normalize(rec.pos - m_normal);
    rec.frontFace = glm::dot(ray.getDirection(), m_normal) < 0;
    if (rec.frontFace)
        rec.normal = m_normal;
    else
        rec.normal = -m_normal;

    return true;
}

bool Quad::isInterior(double a, double b, HitRecord& rec) const
{
    Interval unit_interval = Interval(0, 1);

    if (!unit_interval.contains(a) || !unit_interval.contains(b))
        return false;

    rec.u = a;
    rec.v = b;
    return true;
}

HittableList::Ptr Quad::box(const glm::dvec3& a, const glm::dvec3& b, const Material::Ptr& material)
{
    auto sides = std::make_shared<HittableList>();

    auto min = glm::dvec3(std::fmin(a.x, b.x), std::fmin(a.y, b.y), std::fmin(a.z, b.z));
    auto max = glm::dvec3(std::fmax(a.x, b.x), std::fmax(a.y, b.y), std::fmax(a.z, b.z));

    auto dx = glm::dvec3(max.x - min.x, 0, 0);
    auto dy = glm::dvec3(0, max.y - min.y, 0);
    auto dz = glm::dvec3(0, 0, max.z - min.z);

    sides->add(std::make_shared<Quad>(glm::dvec3(min.x, min.y, max.z), dx, dy, material)); // front
    sides->add(std::make_shared<Quad>(glm::dvec3(max.x, min.y, max.z), -dz, dy, material)); // right
    sides->add(std::make_shared<Quad>(glm::dvec3(max.x, min.y, min.z), -dx, dy, material)); // back
    sides->add(std::make_shared<Quad>(glm::dvec3(min.x, min.y, min.z), dz, dy, material)); // left
    sides->add(std::make_shared<Quad>(glm::dvec3(min.x, max.y, max.z), dx, -dz, material)); // top
    sides->add(std::make_shared<Quad>(glm::dvec3(min.x, min.y, min.z), dx, dz, material)); // bottom

    return sides;
}

