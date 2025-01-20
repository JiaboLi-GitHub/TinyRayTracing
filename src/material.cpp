#include "material.h"
#include <qdebug.h>

Lambertian::Lambertian(const glm::dvec3& albedo)
	: Lambertian(std::make_shared<SolidColorTexture>(albedo))
{
}

Lambertian::Lambertian(const Texture::Ptr& texture)
	: m_texture(texture)
{
}

bool Lambertian::scatter(const Ray& rayIn, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered) const
{
	glm::dvec3 scatterDir = rec.normal + randomNormalizeVec3();
	if (nearZero(scatterDir))
		scatterDir = rec.normal;
	scatterDir = glm::normalize(scatterDir);

	scattered = Ray(rec.pos, scatterDir, rayIn.getTime());
	attenuation = m_texture->value(rec.u, rec.v, rec.pos);

	return true;
}

Metal::Metal(const glm::dvec3& albedo, double fuzz)
	: m_albedo(albedo),
	  m_fuzz(fuzz)
{
}

bool Metal::scatter(const Ray& rayIn, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered) const
{
	glm::dvec3 reflected = reflect(rayIn.getDirection(), rec.normal) + m_fuzz * randomNormalizeVec3();
	reflected = glm::normalize(reflected);

	scattered = Ray(rec.pos, reflected, rayIn.getTime());
	attenuation = m_albedo;

	return true;
}

Dielectric::Dielectric(double ratio)
	: m_ratio(ratio)
{
}

bool Dielectric::scatter(const Ray& rayIn, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered) const
{
	attenuation = glm::dvec3(1.0, 1.0, 1.0);
	double ri = rec.frontFace ? (1.0 / m_ratio) : m_ratio;

	glm::dvec3 unit_direction = glm::normalize(rayIn.getDirection());
	double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
	double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

	bool cannot_refract = ri * sin_theta > 1.0;
	glm::dvec3 direction;

	if (cannot_refract || reflectance(cos_theta, ri) > randomDouble())
		direction = reflect(unit_direction, rec.normal);
	else
		direction = refract(unit_direction, rec.normal, ri);

	scattered = Ray(rec.pos, direction, rayIn.getTime());
	return true;
}

double Dielectric::reflectance(double cosine, double refraction_index)
{
	auto r0 = (1 - refraction_index) / (1 + refraction_index);
	r0 = r0 * r0;
	return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}

glm::dvec3 Material::emitted(double u, double v, const glm::dvec3& pos)
{
	return glm::dvec3(0, 0, 0);
}

DiffuseLightTexture::DiffuseLightTexture(const Texture::Ptr& texture)
	: m_texture(texture)
{
}

DiffuseLightTexture::DiffuseLightTexture(const glm::dvec3& color)
	: m_texture(std::make_shared<SolidColorTexture>(color))
{
}

DiffuseLightTexture::~DiffuseLightTexture()
{
}

bool DiffuseLightTexture::scatter(const Ray& rayIn, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered) const
{
	return false;
}

glm::dvec3 DiffuseLightTexture::emitted(double u, double v, const glm::dvec3& pos)
{
	return m_texture->value(u, v, pos);
}


