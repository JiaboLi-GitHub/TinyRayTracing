#include "bvh.h"

BvhNode::BvhNode(HittableList& hittableList)
	: BvhNode(hittableList.getHittables(),0, hittableList.size())
{

}

BvhNode::BvhNode(HittableList::Ptr& hittableList)
	: BvhNode(hittableList->getHittables(), 0, hittableList->size())
{

}

BvhNode::BvhNode(std::vector<Hittable::Ptr> hittables, size_t start, size_t end)
{
	for (auto& hittablePtr : hittables)
	{
		m_aabb = Aabb(m_aabb, hittablePtr->getAabb());
	}

	size_t hittableSize = end - start;
	if (hittableSize == 1)
	{
		m_l = m_r = hittables[start];
		return;
	}

	auto longestAxis = m_aabb.longestAxis();
	auto comparator = (longestAxis == 0) ? boxXCompare
					: (longestAxis == 1) ? boxYCompare
					: boxZCompare;

	std::sort(hittables.begin() + start, hittables.begin() + end, comparator);

	if (hittableSize == 2)
	{
		m_l = hittables[start];
		m_r = hittables[start + 1];
	}
	else
	{
		int mid = (start + end) >> 1;
		m_l = std::make_shared<BvhNode>(hittables, start, mid);
		m_r = std::make_shared<BvhNode>(hittables, mid, end);
	}
}

bool BvhNode::hit(const Ray& ray, Interval rayT, HitRecord& rec) const
{
	if (!m_aabb.hit(ray, rayT))
		return false;

	bool hitLeft = m_l->hit(ray, rayT, rec);
	bool hitRight = m_r->hit(ray, Interval(rayT.getMin(), hitLeft ? rec.t : rayT.getMax()), rec);

	return hitLeft || hitRight;
}

bool BvhNode::boxCompare(const Hittable::Ptr a, const Hittable::Ptr b, int axisIndex)
{
	auto aInterval = a->getAabb().axisInterval(axisIndex);
	auto bInterval = b->getAabb().axisInterval(axisIndex);
	return aInterval.getMin() < bInterval.getMin();
}

bool BvhNode::boxXCompare(const Hittable::Ptr a, const Hittable::Ptr b)
{
	return BvhNode::boxCompare(a, b, 0);
}

bool BvhNode::boxYCompare(const Hittable::Ptr a, const Hittable::Ptr b)
{
	return BvhNode::boxCompare(a, b, 1);
}

bool BvhNode::boxZCompare(const Hittable::Ptr a, const Hittable::Ptr b)
{
	return BvhNode::boxCompare(a, b, 2);
}
