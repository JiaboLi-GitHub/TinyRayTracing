#pragma once

#include <QObject>
#include "global.h"
#include "ray.h"
#include "hittable.h"

struct CameraConfig
{
	glm::dvec3 cameraCenter;
	double fov;
	glm::dvec3 look;
	glm::dvec3 up;
	int samplesPerPixel;			// 每个像素的随机采样次数
	int maxDepth;					// 递归深度
	double defocusAngle;			// 光线通过屏幕的变化角度
	double focusDist;				// 焦距
	glm::dvec3 backgroundColor;		// 背景颜色
};

class Camera  : public QObject
{
	Q_OBJECT

public:
	Camera(QObject* parent = nullptr);

	~Camera();

	void init(const CameraConfig& config);

	void render(const Hittable::Ptr& scene);

private:
	glm::dvec3 rayColor(const Ray& ray, const Hittable::Ptr& scene, int depth);

	glm::dvec3 sampleSquare() const;

	Ray getRay(int i, int j) const;

	glm::dvec3 defocusDiskSample() const;

signals:
	void sigRender(int u, int v, const glm::dvec3& color);

private:
	glm::dvec3 m_cameraCenter;
	double m_fov;
	glm::dvec3 m_look;
	glm::dvec3 m_up;
	double m_aspectRatio;							// 图像长宽比
	int m_imageWidth = CanvasWidth;					// 图片长度
	int m_imageHeight;								// 图片宽度
	glm::dvec3 m_pixelDeltaU;
	glm::dvec3 m_pixelDeltaV;
	glm::dvec3 m_pixel00Loc;
	int m_samplesPerPixel;							// 每个像素的随机采样次数
	int m_maxDepth;									// 最大递归深度
	double m_defocusAngle;							// 光线通过屏幕的变化角度
	double m_focusDist;								// 焦距
	glm::dvec3 m_defocusDiskU;				
	glm::dvec3 m_defocusDiskV;
	glm::dvec3 m_backgroundColor;					// 背景颜色
};
