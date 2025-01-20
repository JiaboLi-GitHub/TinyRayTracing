#pragma once
#include "global.h"
#include "interval.h"
#include "ray.h"

class Aabb
{
public:
	Aabb();

	Aabb(const Interval& x, const Interval& y, const Interval& z);

	Aabb(const glm::dvec3& a, const glm::dvec3& b);

	Aabb(const Aabb& box0, const Aabb& box1);

	// 相交测试
	bool hit(const Ray& r, const Interval& rayT) const;

	Interval getX() const;

	Interval getY() const;
	
	Interval getZ() const;

	int longestAxis() const;

	// 根据轴获取区间
	const Interval& axisInterval(int n) const;

	Aabb operator+(const glm::dvec3& offset) const;

private:
	// 确保数据合法
	void ensureLegality();

private:
	Interval m_x;
	Interval m_y;
	Interval m_z;
};

