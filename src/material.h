#pragma once
#include "global.h"
#include "ray.h"
#include "hittable.h"
#include "texture.h"

// 材质
class Material
{
public:
	using Ptr = std::shared_ptr<Material>;

	virtual ~Material() = default;

	/*
	 * 计算光线作用于材质的散射作用
	 * @param rayIn			入射光线
	 * @param rec			相交结果
	 * @param attenuation	衰减系数
	 * @param scattered		散射光线
	 * @return				是否成功
	 */
	virtual bool scatter(const Ray& rayIn, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered) const = 0;

	virtual glm::dvec3 emitted(double u, double v, const glm::dvec3& pos);
};

// 朗伯模型
class Lambertian : public Material
{
public:
	Lambertian(const glm::dvec3& albedo);

	Lambertian(const Texture::Ptr& texture);

	bool scatter(const Ray& rayIn, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered) const override;

private:
	Texture::Ptr m_texture;
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

// 灯光
class DiffuseLightTexture : public Material
{
public:
	DiffuseLightTexture(const Texture::Ptr& texture);
	
	DiffuseLightTexture(const glm::dvec3& color);

	~DiffuseLightTexture();

	bool scatter(const Ray& rayIn, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered) const override;

	glm::dvec3 emitted(double u, double v, const glm::dvec3& pos);

private:
	Texture::Ptr m_texture;
};
