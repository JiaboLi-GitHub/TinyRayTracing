#include "material.cuh"

__device__ Material::Material()
{
}

__device__ Material::~Material()
{
}

__device__ bool Material::scatter(const Ray& rayIn, const HitRecord& rec, vec3& attenuation, Ray& scattered) const
{
    attenuation = vec3(1.0, 1.0, 1.0);
    scattered = Ray(vec3(0.5,0.1,0.6), vec3(0.1,0.2,0.5));
    return true;
}

__device__ Lambertian::Lambertian(const vec3& albedo)
    : m_albedo(albedo)
{
}

__device__ bool Lambertian::scatter(const Ray& rayIn, const HitRecord& rec, vec3& attenuation, Ray& scattered) const
{
    vec3 scatterDir = rec.normal + randomNormalizeVec3();
    if (nearZero(scatterDir))
        scatterDir = rec.normal;
    scatterDir = unit_vector(scatterDir);

    scattered = Ray(rec.pos, scatterDir);
    attenuation = m_albedo;

    return true;
}

__device__ Metal::Metal(const vec3& albedo, double fuzz)
    : m_albedo(albedo),
      m_fuzz(fuzz)
{
}

__device__ bool Metal::scatter(const Ray& rayIn, const HitRecord& rec, vec3& attenuation, Ray& scattered) const
{
    vec3 reflected = reflect(rayIn.getDirection(), rec.normal) + m_fuzz * randomNormalizeVec3();
    reflected = unit_vector(reflected);

    scattered = Ray(rec.pos, reflected);
    attenuation = m_albedo;

    return true;
}

__device__ Dielectric::Dielectric(double ratio)
    : m_ratio(ratio)
{
}

__device__ bool Dielectric::scatter(const Ray& rayIn, const HitRecord& rec, vec3& attenuation, Ray& scattered) const
{
    attenuation = vec3(1.0, 1.0, 1.0);
    double ri = rec.frontFace ? (1.0 / m_ratio) : m_ratio;

    vec3 unit_direction = unit_vector(rayIn.getDirection());
    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = ri * sin_theta > 1.0;
    vec3 direction;

    if (cannot_refract || reflectance(cos_theta, ri) > randomDouble())
        direction = reflect(unit_direction, rec.normal);
    else
        direction = refract(unit_direction, rec.normal, ri);

    scattered = Ray(rec.pos, direction);
    return true;
}

__device__ double Dielectric::reflectance(double cosine, double refraction_index)
{
    auto r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}
