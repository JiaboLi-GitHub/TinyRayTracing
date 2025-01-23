#ifndef CAMERA_CUH
#define CAMERA_CUH

#include "global.cuh"
#include "ray.cuh"
#include "hittable.cuh"

class Camera
{
public:
    __device__ Camera(vec3* image);

    __device__ void render(Hittable* scene, int x, int y);

private:
    __device__ void init();

    __device__ vec3 rayColor(const Ray& ray, const Hittable* scene);

    __device__ Ray getRay(int i, int j) const;

    __device__ vec3 sampleSquare() const;

    __device__ vec3 defocusDiskSample() const;

private:
    vec3* m_image = nullptr;

    vec3 m_cameraCenter;
    double m_fov;
    vec3 m_look;
    vec3 m_up;
    double m_aspectRatio;
    int m_imageWidth;
    int m_imageHeight;
    vec3 m_pixelDeltaU;
    vec3 m_pixelDeltaV;
    vec3 m_pixel00Loc;
    int m_samplesPerPixel; // 每个像素的随机采样次数
    int m_maxDepth; // 最大递归深度
    double m_defocusAngle; // 光线通过屏幕的变化角度
    double m_focusDist; // 焦距
    vec3 m_defocusDiskU;
    vec3 m_defocusDiskV;
};

#endif //CAMERA_CUH
