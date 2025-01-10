#include "camera.h"
#include <cmath>
#include <qdebug.h>
#include "sphere.h"

Camera::Camera(QObject *parent)
	: QObject(parent)
{
	init();
}

Camera::~Camera()
{}

void Camera::render(Hittable::Ptr scene)
{
	for (int j = 0; j < m_imageHeight; ++j)
	{
		for (int i = 0; i < m_imageWidth; ++i)
		{
			glm::dvec3 color = glm::dvec3(0.0);
			for (int sample = 0; sample < m_samplesPerPixel; ++sample)
			{
				Ray ray = getRay(i, j);
				color += rayColor(ray, scene, 0);
			}
			color /= m_samplesPerPixel;

			emit sigRender(i, j, color);
		}
	}
}

void Camera::init()
{
	m_imageHeight= int(CanvasWidth / AspectRatio);

	//double focalLength = glm::length(m_look - m_cameraCenter);
	//double h = std::tan(toRadians(m_fov) / 2);
	double viewportHeight = 2.0 * m_focusDist * std::tan(toRadians(m_fov) / 2);
	double viewportWidth = viewportHeight * (double(m_imageWidth) / m_imageHeight);

	glm::dvec3 w, u, v;
	w = glm::normalize(m_cameraCenter - m_look);
	u = glm::normalize(glm::cross(m_up, w));
	v = glm::cross(w, u);

	glm::dvec3 viewPortU = viewportWidth * u;
	glm::dvec3 viewPortV = viewportHeight * v;

	m_pixelDeltaU = viewPortU / (m_imageWidth * 1.0);
	m_pixelDeltaV = viewPortV / (m_imageHeight * 1.0);

	auto pixel_upper_left = m_cameraCenter - m_focusDist * w - viewPortU / 2.0 - viewPortV / 2.0;
	m_pixel00Loc = pixel_upper_left + 0.5 * (m_pixelDeltaU + m_pixelDeltaV);

	auto defocusRadius = m_focusDist * tan(toRadians(m_defocusAngle / 2));
	m_defocusDiskU = u * defocusRadius;
	m_defocusDiskV = v * defocusRadius;
}

glm::dvec3 Camera::rayColor(const Ray& ray, const Hittable::Ptr& scene, int depth)
{
	if (depth == m_maxDepth)
		return glm::dvec3(0.0, 0.0, 0.0);

	HitRecord rec;
	if (scene->hit(ray, Interval(0.001, Infinity), rec))
	{
		Ray scattered;
		glm::dvec3 attenuation;
		if (rec.material->scatter(ray, rec, attenuation, scattered))
		{
			return attenuation * rayColor(scattered, scene, depth + 1);
		}
		return glm::dvec3(0.0, 0.0, 0.0);
	}

	double x = 0.5f * (ray.getDirection().y + 1.0);
	auto backColor = (1.0 - x) * glm::dvec3(1.0) + x * glm::dvec3(0.5f, 0.7f, 1.0);
	return backColor;
}

glm::dvec3 Camera::sampleSquare() const
{
	return glm::dvec3(randomdouble() - 0.5, randomdouble() - 0.5, 0);
}

Ray Camera::getRay(int i, int j) const
{
	glm::dvec3 offset = sampleSquare();

	glm::dvec3 pixelCenter = m_pixel00Loc + (i + offset.x) * m_pixelDeltaU + (j + offset.y) * m_pixelDeltaV;

	glm::dvec3 rayOrigin = (m_defocusAngle <= 0) ? m_cameraCenter : defocusDiskSample();
	glm::dvec3 rayDirection = pixelCenter - rayOrigin;

	rayDirection = glm::normalize(rayDirection);

	return Ray(rayOrigin, rayDirection);
}

glm::dvec3 Camera::defocusDiskSample() const
{
	glm::dvec3 vec3 = randomDisk();
	return m_cameraCenter + (vec3.x * m_defocusDiskU) + (vec3.y * m_defocusDiskV);
}

