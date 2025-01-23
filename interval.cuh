#ifndef INTERVAL_CUH
#define INTERVAL_CUH

#include "global.cuh"

class Interval
{
public:
    __device__ Interval(double min,double max);

    __device__ ~Interval();

    __device__ double size() const;

    __device__ bool contains(double x) const;

    __device__ bool surrounds(double x) const;

    __device__ double clamp(double x) const;

    __device__ double getMin() const;

    __device__ double getMax() const;

private:
    double m_min;
    double m_max;
};



#endif //INTERVAL_CUH
