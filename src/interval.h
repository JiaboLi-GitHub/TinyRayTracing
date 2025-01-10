#pragma once
#include "global.h"

class Interval
{
public:
	Interval();

	Interval(double min, double max);

	double size() const;

	bool contains(double x) const;

	bool surrounds(double x) const;

	double clamp(double x) const;

	double getMin() const;

	double getMax() const;

	void setMin(double min);

	void setMax(double max);

private:
	double m_min = Infinity;
	double m_max = -Infinity;
};

