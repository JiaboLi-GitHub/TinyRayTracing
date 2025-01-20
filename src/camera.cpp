#include "camera.h"
#include <cmath>
#include <qthreadpool.h>
#include <qfuture.h>
#include <QtConcurrent>
#include <qdebug.h>
#include "sphere.h"

Camera::Camera(QObject *parent)
	: QObject(parent)
{
}

Camera::~Camera()
{}

void Camera::init(const CameraConfig & config)
{
	// 拷贝
	m_aspectRatio = AspectRatio; 
	m_imageWidth = CanvasWidth;
	m_cameraCenter = config.cameraCenter;
	m_fov = config.fov;
	m_look = config.look;         
	m_up = config.up;             
	m_samplesPerPixel = config.samplesPerPixel;
	m_maxDepth = config.maxDepth;    
	m_defocusAngle = config.defocusAngle;
	m_focusDist = config.focusDist; 
	m_backgroundColor = config.backgroundColor;

	// 解析
	m_imageHeight = int(CanvasWidth / AspectRatio);

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

void Camera::render(const Hittable::Ptr& scene)
{
	auto rayColorFun = [this](const Hittable::Ptr& scene, int i, int j) {
		Ray ray = getRay(i, j);
		return rayColor(ray, scene, 0);
		};

	std::unique_ptr<QThreadPool> threadPool = std::make_unique<QThreadPool>();
	threadPool->setMaxThreadCount(32);
	QVector<QFuture<glm::dvec3>> futures;
	futures.resize(m_samplesPerPixel);

	for (int j = 0; j < m_imageHeight; ++j)
	{
		for (int i = 0; i < m_imageWidth; ++i)
		{
			glm::dvec3 color = glm::dvec3(0.0);
			for (int sample = 0; sample < m_samplesPerPixel; ++sample)
			{
				futures[sample] = QtConcurrent::run(threadPool.get(), rayColorFun, scene, i, j);
			}

			for (int sample = 0; sample < m_samplesPerPixel; ++sample)
			{
				futures[sample].waitForFinished();
				color += futures[sample].result();
			}

			color /= m_samplesPerPixel;

			emit sigRender(i, j, color);
		}
	}
}

glm::dvec3 Camera::rayColor(const Ray& ray, const Hittable::Ptr& scene, int depth)
{
	if (depth == m_maxDepth)
		return glm::dvec3(0.0, 0.0, 0.0);

	HitRecord rec;

	// If the ray hits nothing, return the background color.
	if (!scene->hit(ray, Interval(0.001, Infinity), rec))
		return m_backgroundColor;

	Ray scattered;
	glm::dvec3 attenuation;
	glm::dvec3 color_from_emission = rec.material->emitted(rec.u, rec.v, rec.pos);

	if (!rec.material->scatter(ray, rec, attenuation, scattered))
		return color_from_emission;

	glm::dvec3 color_from_scatter = attenuation * rayColor(scattered, scene, depth + 1);

	return color_from_emission + color_from_scatter;
}

glm::dvec3 Camera::sampleSquare() const
{
	return glm::dvec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
}

Ray Camera::getRay(int i, int j) const
{
	glm::dvec3 offset = sampleSquare();

	glm::dvec3 pixelCenter = m_pixel00Loc + (i + offset.x) * m_pixelDeltaU + (j + offset.y) * m_pixelDeltaV;

	glm::dvec3 rayOrigin = (m_defocusAngle <= 0) ? m_cameraCenter : defocusDiskSample();
	glm::dvec3 rayDirection = pixelCenter - rayOrigin;

	rayDirection = glm::normalize(rayDirection);

	double time = randomDouble();

	return Ray(rayOrigin, rayDirection, time);
}

glm::dvec3 Camera::defocusDiskSample() const
{
	glm::dvec3 vec3 = randomDisk();
	return m_cameraCenter + (vec3.x * m_defocusDiskU) + (vec3.y * m_defocusDiskV);
}

