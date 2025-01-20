#pragma once
#include <QImage>
#include "global.h"

class Texture
{
public:
	using Ptr = std::shared_ptr<Texture>;

	virtual ~Texture() = default;

	virtual glm::dvec3 value(double u, double v, const glm::dvec3& p) const = 0;
};

// 固定颜色纹理
class SolidColorTexture : public Texture
{
public:
	using Ptr = std::shared_ptr<SolidColorTexture>;

	SolidColorTexture(const glm::dvec3& color);

	SolidColorTexture(double r, double g, double b);

	glm::dvec3 value(double u, double v, const glm::dvec3& p) const override;

private:
	glm::dvec3 m_color;
};

// 棋盘纹理
class CheckerTexture : public Texture
{
public:
	using Ptr = std::shared_ptr<CheckerTexture>;

	CheckerTexture(double scale, const Texture::Ptr& even, const Texture::Ptr& odd);

	CheckerTexture(double scale, const glm::dvec3& even, const glm::dvec3& odd);

	glm::dvec3 value(double u, double v, const glm::dvec3& p) const override;

private:
	double m_invScale;
	Texture::Ptr m_even;
	Texture::Ptr m_odd;
};

class ImageTexture : public Texture
{
public:
	using Ptr = std::shared_ptr<ImageTexture>;

	ImageTexture(const char* filePath);

	glm::dvec3 value(double u, double v, const glm::dvec3& pos) const override;

private:
	QImage m_image;
};
