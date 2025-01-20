#include "interval.h"

Interval::Interval()
{
}

Interval::Interval(double min, double max)
	: m_min(min),
	  m_max(max)
{
}

Interval::Interval(const Interval& a, const Interval& b)
{
	m_min = std::min(a.getMin(), b.getMin());
	m_max = std::max(a.getMax(), b.getMax());
}

double Interval::size() const
{
	return m_max - m_min;
}

bool Interval::contains(double x) const
{
	return m_min <= x && x <= m_max;
}

bool Interval::surrounds(double x) const
{
	return m_min < x && x < m_max;
}

double Interval::clamp(double x) const
{
	if (x < m_min)
		return m_min;

	if (x > m_max)
		return m_max;

	return x;
}

double Interval::getMin() const
{
	return m_min;
}

double Interval::getMax() const
{
	return m_max;
}

void Interval::setMin(double min)
{
	m_min = min;
}

void Interval::setMax(double max)
{
	m_max = max;
}

Interval Interval::expand(double delta) const
{
	auto padding = delta / 2;
	return Interval(m_min - padding, m_max + padding);
}

Interval Interval::operator+(double displacement) const
{
	return Interval(m_min + displacement, m_max + displacement);
}
