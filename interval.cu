#include "interval.cuh"

__device__ Interval::Interval(double min, double max)
    : m_min(min), m_max(max)
{
}

__device__ Interval::~Interval()
{
}

__device__ double Interval::size() const
{
    return m_max - m_min;
}

__device__ bool Interval::contains(double x) const
{
    return m_min <= x && x <= m_max;
}

__device__ bool Interval::surrounds(double x) const
{
    return m_min < x && x < m_max;
}

__device__ double Interval::clamp(double x) const
{
    if (x < m_min) return m_min;
    if (x > m_max) return m_max;
    return x;
}

__device__ double Interval::getMin() const
{
    return m_min;
}

__device__ double Interval::getMax() const
{
    return m_max;
}
