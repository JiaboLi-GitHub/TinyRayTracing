#ifndef MATERIAL_CUH
#define MATERIAL_CUH

#include "ray.cuh"
#include  "hittable.cuh"

class Material
{
public:
    __device__ Material();

    __device__ virtual ~Material();

    __device__ virtual bool scatter(const Ray& rayIn, const HitRecord& rec, vec3& attenuation, Ray& scattered) const;
};

// 朗伯模型
class Lambertian : public Material
{
public:
    __device__ Lambertian(const vec3& albedo);

    __device__ bool scatter(const Ray& rayIn, const HitRecord& rec, vec3& attenuation, Ray& scattered) const override;

private:
    vec3 m_albedo;	// 反照率
};

// 金属
class Metal : public Material
{
public:
    __device__ Metal(const vec3& albedo, double fuzz);

    __device__ bool scatter(const Ray& rayIn, const HitRecord& rec, vec3& attenuation, Ray& scattered) const override;

private:
    vec3 m_albedo;	        // 反照率
    double m_fuzz;			// 模糊参数
};

// 介质
class Dielectric : public Material
{
public:
    __device__ Dielectric(double ratio);

    __device__ bool scatter(const Ray& rayIn, const HitRecord& rec, vec3& attenuation, Ray& scattered) const override;

private:
    __device__ static double reflectance(double cosine, double refraction_index);

private:
    double m_ratio;			// 折射率比值
};

#endif //MATERIAL_CUH
