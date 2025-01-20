#pragma once
#include "global.h"
#include "hittable.h"

class HittableList : public Hittable
{
public:
	using Ptr = std::shared_ptr<HittableList>;

	HittableList();

	HittableList(Hittable::Ptr hittable);

	virtual bool hit(const Ray& ray, Interval rayT, HitRecord& rec) const override;

	void add(const Hittable::Ptr& hittable);

	void clear();

	std::vector<Hittable::Ptr> getHittables();

	size_t size() const;

private:
	std::vector<Hittable::Ptr> m_hittables;
};

