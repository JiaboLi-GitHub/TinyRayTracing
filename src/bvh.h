#pragma once
#include "hittable.h"
#include "hittableList.h"

class BvhNode : public Hittable
{
public:
	using Ptr = std::shared_ptr<BvhNode>;

	BvhNode(HittableList& hittableList);

	BvhNode(HittableList::Ptr& hittableList);

	BvhNode(std::vector<Hittable::Ptr> hittables, size_t start, size_t end);

	bool hit(const Ray& ray, Interval rayT, HitRecord& rec) const override;

private:
    static bool boxCompare(const Hittable::Ptr a, const Hittable::Ptr b, int axisIndex);

    static bool boxXCompare(const Hittable::Ptr a, const Hittable::Ptr b);

	static bool boxYCompare(const Hittable::Ptr a, const Hittable::Ptr b);

	static bool boxZCompare(const Hittable::Ptr a, const Hittable::Ptr b);

private:
	Hittable::Ptr m_l;
	Hittable::Ptr m_r;
};

