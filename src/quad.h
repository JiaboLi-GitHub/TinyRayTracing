#pragma once
#include "hittable.h"
#include "hittableList.h"
#include "aabb.h"
#include "material.h"

class Quad : public Hittable
{
public:
	using Ptr = std::shared_ptr<Quad>;

	Quad(glm::dvec3 q, glm::dvec3 u, glm::dvec3 v, Material::Ptr material);

	void setBoundingBox();

	bool hit(const Ray& ray, Interval rayT, HitRecord& rec) const override;

	virtual bool isInterior(double a, double b, HitRecord& rec) const;

	static HittableList::Ptr box(const glm::dvec3& a, const glm::dvec3& b, const Material::Ptr& material);

private:
	glm::dvec3 m_q;
	glm::dvec3 m_u;
	glm::dvec3 m_v;
	Material::Ptr m_material;
	glm::dvec3 m_normal;
	double m_d;
	glm::dvec3 m_w;
};

