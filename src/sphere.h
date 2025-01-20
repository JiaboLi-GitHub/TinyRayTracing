#pragma once
#include "hittable.h"
#include "material.h"

// 球体
class Sphere : public Hittable
{
public:
	using Ptr = std::shared_ptr<Sphere>;

	Sphere(glm::dvec3 center, double r, Material::Ptr material);

	Sphere(const glm::dvec3& center, double r, const glm::dvec3& speed, const Material::Ptr& material);

	~Sphere();

	glm::dvec3 position(double time) const;

	bool hit(const Ray& ray, Interval rayT, HitRecord& rec) const override;

private:
	static void getSphereUV(const glm::dvec3& pos, double& u, double& v);

private:
	glm::dvec3 m_center;								// 圆心
	glm::dvec3 m_speed = glm::dvec3(0, 0, 0);			// 速度
	double m_r;											// 半径
	Material::Ptr m_material = nullptr;					// 材质
};

