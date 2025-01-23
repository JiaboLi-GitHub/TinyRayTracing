#include "camera.cuh"
#include <cuda_runtime.h>
#include "material.cuh"

__device__ Camera::Camera(vec3* image)
    : m_image(image)
{
    init();
}

__device__ void Camera::render(Hittable* scene, int x, int y)
{
    vec3 color = vec3(0, 0, 0);
    for (int i = 0; i < m_samplesPerPixel; ++i)
    {
        Ray ray = getRay(x, y);
        color += rayColor(ray, scene);
    }
    color /= m_samplesPerPixel;

    m_image[x + y * ImageWidthD] = color;
    //m_image[x+y*ImageWidthD] = vec3(0.5, 0.8, 0.9);
    // printf("x = %d, y = %d:  %lf %lf %lf \n", x,y,color.r(),color.g(),color.b());
}

__device__ void Camera::init()
{
    m_cameraCenter = vec3(13, 2, 3);
    m_fov = 20;
    m_look = vec3(0, 0, 0);
    m_up = vec3(0, 1, 0);
    m_aspectRatio = AspectRatioD;
    m_imageWidth = ImageWidthD;
    m_pixelDeltaU = vec3(0, 0, 0);
    m_pixelDeltaV = vec3(0, 0, 0);
    m_pixel00Loc = vec3(0, 0, 0);
    m_samplesPerPixel = 10; // 每个像素的随机采样次数
    m_maxDepth = 5; // 最大递归深度
    m_defocusAngle = 0.6; // 光线通过屏幕的变化角度
    m_focusDist = 10.0; // 焦距

    m_imageHeight = int(ImageWidthD / AspectRatioD);
    double viewportHeight = 2.0 * m_focusDist * tan(toRadians(m_fov) / 2);
    double viewportWidth = viewportHeight * (double(m_imageWidth) / m_imageHeight);

    vec3 w, u, v;
    w = unit_vector(m_cameraCenter - m_look);
    u = unit_vector(cross(m_up, w));
    v = cross(w, u);

    vec3 viewPortU = viewportWidth * u;
    vec3 viewPortV = viewportHeight * v;

    m_pixelDeltaU = viewPortU / (m_imageWidth * 1.0);
    m_pixelDeltaV = viewPortV / (m_imageHeight * 1.0);

    auto pixel_upper_left = m_cameraCenter - m_focusDist * w - viewPortU / 2.0 - viewPortV / 2.0;
    m_pixel00Loc = pixel_upper_left + 0.5 * (m_pixelDeltaU + m_pixelDeltaV);

    auto defocusRadius = m_focusDist * tan(toRadians(m_defocusAngle / 2));
    m_defocusDiskU = u * defocusRadius;
    m_defocusDiskV = v * defocusRadius;
}

__device__ vec3 Camera::rayColor(const Ray& ray, const Hittable* scene)
{
    Ray currentRay = ray;
    vec3 attenuation(1.0, 1.0, 1.0); // 初始衰减
    vec3 finalColor(0.0, 0.0, 0.0); // 最终颜色
    int depth = 0;

    while (depth < m_maxDepth)
    {
        HitRecord rec;
        if (scene->hit(currentRay, Interval(0.001, InfinityD), rec))
        {
            Ray scattered;
            vec3 newAttenuation;

            // 处理散射
            if (rec.material->scatter(currentRay, rec, newAttenuation, scattered))
            {
                // 更新衰减和光线
                attenuation *= newAttenuation;
                currentRay = scattered;
                depth++;
            }
            else
            {
                // 如果没有散射，结束循环
                break;
            }
        }
        else
        {
            // 计算背景颜色
            double x = 0.5f * (currentRay.getDirection().y() + 1.0);
            vec3 backColor = (1.0 - x) * vec3(1.0, 1.0, 1.0) + x * vec3(0.5f, 0.7f, 1.0);
            finalColor += backColor * attenuation; // 加权背景颜色
            break; // 结束循环
        }
    }

    return finalColor;
}

__device__ Ray Camera::getRay(int i, int j) const
{
    vec3 offset = sampleSquare();

    vec3 pixelCenter = m_pixel00Loc + (i + offset.x()) * m_pixelDeltaU + (j + offset.y()) * m_pixelDeltaV;

    vec3 rayOrigin = (m_defocusAngle <= 0) ? m_cameraCenter : defocusDiskSample();
    vec3 rayDirection = pixelCenter - rayOrigin;

    rayDirection = unit_vector(rayDirection);

    return Ray(rayOrigin, rayDirection);
}

__device__ vec3 Camera::sampleSquare() const
{
    return vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
}

__device__ vec3 Camera::defocusDiskSample() const
{
    vec3 vec3 = randomDisk();
    return m_cameraCenter + (vec3.x() * m_defocusDiskU) + (vec3.y() * m_defocusDiskV);
}
