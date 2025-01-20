#pragma once
#include "global.h"
#include "ray.h"
#include "interval.h"
#include "aabb.h"

// 碰撞结果
class Material;
struct HitRecord
{
	glm::dvec3 pos;							// 位置
	glm::dvec3 normal;						// 法向量
	double t;				
	bool frontFace;							// 朝向
	std::shared_ptr<Material> material;		// 材质
	double u;
	double v;
};

class Hittable
{
public:
	using Ptr = std::shared_ptr<Hittable>;

	virtual ~Hittable() = default;

	virtual bool hit(const Ray& ray, Interval rayT, HitRecord& rec) const = 0;

	Aabb getAabb();

protected:
	Aabb m_aabb;	// 包围盒
};

class Translate : public Hittable
{
public:
	Translate(const Hittable::Ptr& hittable, const glm::dvec3& offset);

	bool hit(const Ray& ray, Interval rayT, HitRecord& rec) const override;

private:
	Hittable::Ptr m_hittable;
	glm::dvec3 m_offset;
};

class RotateY : public Hittable {
public:
	RotateY(const Hittable::Ptr& hittable, double angle);

	bool hit(const Ray& ray, Interval rayT, HitRecord& rec) const override;

private:
	Hittable::Ptr m_hittable;
    double m_sinTheta;
    double m_cosTheta;
};
