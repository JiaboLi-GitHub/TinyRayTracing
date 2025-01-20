#include "canvas.h"
#include <QPaintEvent>
#include <QPainter>
#include <QGuiApplication>
#include <QTimer>
#include <QDir>

Canvas::Canvas(QWidget* parent)
	: QWidget(parent)
{
	initUi();
	initBitmap();
}

Canvas::~Canvas()
{
}

void Canvas::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
	QPainter painterPixmap(&m_pixmap);

	for (const auto& [u,v] : m_changeBit)
	{
		int x = u, y = height() - v - 1;

		glm::dvec3 colorVec3 = gammaCorrection(m_bitmap[u][v]);

		QColor color = QColor::fromRgbF(colorVec3.r, colorVec3.g, colorVec3.b);
		
		painter.setPen(color);
		painter.drawPoint(x, y);

		painterPixmap.setPen(color);
		painterPixmap.drawPoint(x, y);
	}

	m_changeBit.clear();
}

void Canvas::closeEvent(QCloseEvent* event)
{
	QWidget::closeEvent(event);
	
	emit sigClose();
}

void Canvas::initUi()
{
	setAttribute(Qt::WA_NoSystemBackground);

	setFixedSize({ CanvasWidth ,int(CanvasWidth / AspectRatio)});
	m_pixmap = QPixmap(size());

	QScreen* screen = QGuiApplication::primaryScreen();
	if (screen) 
	{
		m_refreshRate = screen->refreshRate();
		qInfo() << "屏幕刷新率:" << m_refreshRate;

		// 设置定时器
		QTimer* timer = new QTimer(this);
		connect(timer, &QTimer::timeout, [this]() {
			update();
			});
		timer->start(1000/ m_refreshRate);
	}
}

void Canvas::initBitmap()
{
	m_bitmap.resize(CanvasWidth);
	for (int i = 0; i < CanvasWidth; ++i)
		m_bitmap[i].resize(int(CanvasWidth / AspectRatio));
}

glm::dvec3 Canvas::gammaCorrection(const glm::dvec3 color)
{
	auto correctionFun = [](const double& x) -> double
		{
			if (x > 0.0)
				return std::sqrt(x);

			return 0.0;
		};
	
	double r, g, b;
	r = correctionFun(color.x);
	g = correctionFun(color.y);
	b = correctionFun(color.z);

	return glm::dvec3(r, g, b);
}

void Canvas::saveAsImage()
{
	repaint();

	QString dateTimeString = QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss");
	QString fileName = QString("%1.png").arg(dateTimeString);

	auto code = m_pixmap.save(fileName, "PNG");

	if(code)
		qInfo() << QString("图片已保存:%1/%2").arg(QDir::currentPath()).arg(fileName);
}

void Canvas::slotRenderStart()
{
	m_renderTimer = std::make_shared<QElapsedTimer>();
	m_renderTimer->start();
}

void Canvas::slotRender(int u, int v, glm::dvec3 color)
{
	m_bitmap[u][v] = color;

	m_changeBit.push_back({ u,v });
}

void Canvas::slotRenderEnd()
{
	qint64 elapsed = m_renderTimer->elapsed();
	qInfo() << "渲染耗时：" << elapsed << "毫秒";

	saveAsImage();
}

