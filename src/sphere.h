#pragma once
#include "hittable.h"
#include "material.h"

class Sphere : public Hittable
{
public:
	using Ptr = std::shared_ptr<Sphere>;

	Sphere(glm::dvec3 center, double r, Material::Ptr material);

	~Sphere();

	bool hit(const Ray& ray, Interval rayT, HitRecord& rec) const override;

private:
	glm::dvec3 m_center;					// 圆心
	double m_r;								// 半径
	Material::Ptr m_material = nullptr;		// 材质
};

