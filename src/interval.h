#pragma once
#include "global.h"

class Interval
{
public:
	Interval();

	Interval(double min, double max);

	Interval(const Interval& a, const Interval& b);

	double size() const;

	bool contains(double x) const;

	bool surrounds(double x) const;

	double clamp(double x) const;

	double getMin() const;

	double getMax() const;

	void setMin(double min);

	void setMax(double max);

	Interval expand(double delta) const;

	Interval operator+(double displacement) const;

private:
	double m_min = Infinity;
	double m_max = -Infinity;
};

