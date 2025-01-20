#include "aabb.h"
#include <cmath>

Aabb::Aabb()
{
}

Aabb::Aabb(const Interval& x, const Interval& y, const Interval& z)
	: m_x(x),
	  m_y(y),
	  m_z(z)
{
	ensureLegality();
}

Aabb::Aabb(const glm::dvec3& a, const glm::dvec3& b)
{
	m_x = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
	m_y = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
	m_z = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);

	ensureLegality();
}

Aabb::Aabb(const Aabb& box0, const Aabb& box1)
{
	m_x = Interval(box0.getX(), box1.getX());
	m_y = Interval(box0.getY(), box1.getY());
	m_z = Interval(box0.getZ(), box1.getZ());
}

bool Aabb::hit(const Ray& r, const Interval& rayT) const
{
	Interval intersect = rayT;	//相交区间

	for (int i = 0; i < 3; ++i)
	{
		double d = r.getDirection()[i];
		if (std::fabs(d) < 1e-7)
			return false;

		const Interval& interval = axisInterval(i);
		double o = r.getOrigin()[i];
		double t0 = (interval.getMin() - o) / d;
		double t1 = (interval.getMax() - o) / d;

		if (t0 > t1)
			std::swap(t0, t1);

		intersect.setMin(std::max(t0, intersect.getMin()));
		intersect.setMax(std::min(t1, intersect.getMax()));

		if (intersect.size() <= 0)
			return false;
	}

	return true;
}

Interval Aabb::getX() const
{
	return m_x;
}

Interval Aabb::getY() const
{
	return m_y;
}

Interval Aabb::getZ() const
{
	return m_z;
}

int Aabb::longestAxis() const
{
	int axis = 0;
	
	if (m_y.size() > m_x.size())
		axis = 1;

	if (axis)
	{
		if (m_z.size() > m_y.size())
			axis = 2;
	}
	else
	{
		if (m_z.size() > m_x.size())
			axis = 2;
	}

	return axis;
}

void Aabb::ensureLegality()
{
	double delta = 0.0001;
	if (m_x.size() < delta) 
		m_x = m_x.expand(delta);
	if (m_y.size() < delta) 
		m_y = m_y.expand(delta);
	if (m_z.size() < delta) 
		m_z = m_z.expand(delta);
}

const Interval& Aabb::axisInterval(int n) const
{
	if (n == 1)
		return m_y;
	if (n == 2) 
		return m_z;
	return m_x;
}

Aabb Aabb::operator+(const glm::dvec3& offset) const
{
	return Aabb(m_x + offset.x, m_y + offset.y, m_z + offset.z);
}
