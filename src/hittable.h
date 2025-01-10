#pragma once
#include "global.h"
#include "ray.h"
#include "interval.h"

// 碰撞结果
class Material;
struct HitRecord
{
	glm::dvec3 pos;							// 位置
	glm::dvec3 normal;						// 法向量
	double t;				
	bool frontFace;							// 朝向
	std::shared_ptr<Material> material;		// 材质
};

class Hittable
{
public:
	using Ptr = std::shared_ptr<Hittable>;

	virtual ~Hittable() = default;

	virtual bool hit(const Ray& ray, Interval rayT, HitRecord& rec) const = 0;
};

