#pragma once

#include <QObject>
#include "global.h"
#include "ray.h"
#include "hittable.h"

class Camera  : public QObject
{
	Q_OBJECT

public:
	Camera(QObject* parent = nullptr);
	~Camera();

	void render(Hittable::Ptr scene);

private:
	void init();

	glm::dvec3 rayColor(const Ray& ray, const Hittable::Ptr& scene, int depth);

	glm::dvec3 sampleSquare() const;

	Ray getRay(int i, int j) const;

	glm::dvec3 defocusDiskSample() const;

signals:
	void sigRender(int u, int v, const glm::dvec3& color);

private:
	const glm::dvec3 m_cameraCenter = glm::dvec3(13, 2, 3);
	double m_fov = 20;
	glm::dvec3 m_look = glm::dvec3(0, 0, 0);
	glm::dvec3 m_up = glm::dvec3(0, 1, 0);
	double m_aspectRatio = 16.0 / 9.0;							// 图像长宽比
	int m_imageWidth = CanvasWidth;								// 图片长度
	int m_imageHeight = 0;										// 图片宽度
	glm::dvec3 m_pixelDeltaU = glm::dvec3(0.0);
	glm::dvec3 m_pixelDeltaV = glm::dvec3(0.0);
	glm::dvec3 m_pixel00Loc = glm::dvec3(0.0);
	int m_samplesPerPixel = 10;								// 每个像素的随机采样次数
	int m_maxDepth = 5;										// 最大递归深度
	double m_defocusAngle = 0.6;								// 光线通过屏幕的变化角度
	double m_focusDist = 10.0;									// 焦距
	glm::dvec3   m_defocusDiskU;				
	glm::dvec3   m_defocusDiskV;  
};
