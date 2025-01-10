#pragma once
#include "global.h"
#include "ray.h"
#include "hittable.h"

class Material
{
public:
	using Ptr = std::shared_ptr<Material>;

	virtual ~Material() = default;

	virtual bool scatter(const Ray& rayIn, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered) const = 0;
};

// 朗伯模型
class Lambertian : public Material
{
public:
	Lambertian(const glm::dvec3& albedo);

	bool scatter(const Ray& rayIn, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered) const override;

private:
	glm::dvec3 m_albedo;	// 反照率

};

// 金属
class Metal : public Material
{
public:
	Metal(const glm::dvec3& albedo, double fuzz);

	bool scatter(const Ray& rayIn, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered) const override;

private:
	glm::dvec3 m_albedo;	// 反照率
	double m_fuzz;			// 模糊参数
};

// 介质
class Dielectric : public Material
{
public:
	Dielectric(double ratio);

	bool scatter(const Ray& rayIn, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered) const override;

private:
	static double reflectance(double cosine, double refraction_index);

private:
	double m_ratio;			// 折射率比值
};
