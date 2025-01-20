#include "hittableList.h"

HittableList::HittableList()
{
}

HittableList::HittableList(Hittable::Ptr hittable)
{
	add(hittable);
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

	m_aabb = Aabb(m_aabb, hittable->getAabb());
}

void HittableList::clear()
{
	m_hittables.clear();
}

std::vector<Hittable::Ptr> HittableList::getHittables()
{
	return m_hittables;
}

size_t HittableList::size() const
{
	return m_hittables.size();
}
