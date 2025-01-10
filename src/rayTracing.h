#pragma once

#include <QObject>
#include <glm/glm.hpp>
#include "camera.h"
#include "hittableList.h"

class RayTracing  : public QObject
{
	Q_OBJECT

public:
	RayTracing(QObject* parent = nullptr);

	~RayTracing();

private:
	void initScene();
	void initCamera();

signals:
	void sigRenderStart();
	void sigRender(int u, int v, const glm::dvec3& color);
	void sigRenderEnd();

public slots:
	void slotRender();

private:
	Camera* m_camera = nullptr;
	HittableList::Ptr m_scene = nullptr;
};
