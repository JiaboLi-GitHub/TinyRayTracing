#pragma once
#include "global.h"
#include "hittable.h"

class HittableList : public Hittable
{
public:
	using Ptr = std::shared_ptr<HittableList>;

	HittableList();

	bool hit(const Ray& ray, Interval rayT, HitRecord& rec) const override;

	void add(const Hittable::Ptr& hittable);

	void clear();

private:
	std::vector<Hittable::Ptr> m_hittables;
};

