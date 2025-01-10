#include "interval.h"

Interval::Interval()
{
}

Interval::Interval(double min, double max)
	: m_min(min),
	  m_max(max)
{
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
