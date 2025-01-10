#include "hittableList.h"

HittableList::HittableList()
{
}

bool HittableList::hit(const Ray& ray, Interval rayT, HitRecord& rec) const
{
	HitRecord tmpRec;
	bool hitAnything = false;
	double closestSoFar = rayT.getMax();

	for (const auto& hittable : m_hittables)
	{
		if (hittable->hit(ray, Interval(rayT.getMin(), closestSoFar), tmpRec))
		{
			hitAnything = true;
			closestSoFar = tmpRec.t;
			rec = tmpRec;
		}
	}

	return hitAnything;
}

void HittableList::add(const Hittable::Ptr& hittable)
{
	m_hittables.push_back(hittable);
}

void HittableList::clear()
{
	m_hittables.clear();
}
