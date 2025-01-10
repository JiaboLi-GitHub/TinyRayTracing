#pragma once

#include <QWidget>
#include <glm/glm.hpp>
#include <QVector>
#include <QElapsedTimer>
#include "rayTracing.h"

class Canvas : public QWidget
{
	Q_OBJECT

public:
	Canvas(QWidget* parent = nullptr);

	~Canvas();

protected:
	void paintEvent(QPaintEvent* event) override;

	void closeEvent(QCloseEvent* event) override;

private:
	void initUi();
	
	void initBitmap();
	
	// Gamma矫正
	glm::dvec3 gammaCorrection(const glm::dvec3 color);

	void saveAsImage();

signals:
	void sigClose();

public slots:
	void slotRenderStart();
	
	void slotRender(int u, int v, glm::dvec3 color);
	
	void slotRenderEnd();

private:
	QVector<QVector<glm::dvec3>> m_bitmap;
	QVector<QPair<int, int>> m_changeBit;
	RayTracing* m_rayTracing = nullptr;
	int m_refreshRate = 60;
	std::shared_ptr<QElapsedTimer> m_renderTimer = nullptr;
	QPixmap m_pixmap;
};
