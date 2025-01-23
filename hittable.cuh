#ifndef HITTABLE_CUH
#define HITTABLE_CUH

#include "global.cuh"
#include "ray.cuh"
#include "interval.cuh"

class Material;
struct HitRecord
{
    vec3 pos;
    vec3 normal;
    double t;
    bool frontFace;
    Material* material;

    __device__ HitRecord(){};
    __device__ ~HitRecord(){};
};

class Hittable
{
public:
    __device__ Hittable();

    __device__ virtual ~Hittable();

    __device__ virtual bool hit(const Ray& ray, Interval rayT, HitRecord& rec) const = 0;
};

class HittableList : public Hittable
{
public:
    __device__ HittableList(Hittable** hittables, size_t size);

    __device__ bool hit(const Ray& ray, Interval rayT, HitRecord& rec) const override;

private:
    Hittable** m_hittables;
    size_t m_size;
};

class Sphere : public Hittable
{
public:
    __device__ Sphere(const vec3& center, double r, Material* material);

    __device__ ~Sphere() override;

    __device__ bool hit(const Ray& ray, Interval rayT, HitRecord& rec) const override;

private:
    vec3 m_center;					    // 圆心
    double m_r;							// 半径
    Material* m_material = nullptr;		// 材质
};


#endif //HITTABLE_CUH
