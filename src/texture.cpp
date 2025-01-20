#include "texture.h"
#include "interval.h"

SolidColorTexture::SolidColorTexture(const glm::dvec3& color)
	: m_color(color)
{
}

SolidColorTexture::SolidColorTexture(double r, double g, double b)
	: m_color(glm::dvec3(r, g, b))
{

}

glm::dvec3 SolidColorTexture::value(double u, double v, const glm::dvec3& p) const
{
	return m_color;
}

CheckerTexture::CheckerTexture(double scale, const Texture::Ptr& even, const Texture::Ptr& odd)
	: m_invScale(1.0/ scale),
	  m_even(even),
	  m_odd(odd)
{
}

CheckerTexture::CheckerTexture(double scale, const glm::dvec3& even, const glm::dvec3& odd)
	: CheckerTexture(scale, std::make_shared<SolidColorTexture>(even), std::make_shared<SolidColorTexture>(odd))
{

}

glm::dvec3 CheckerTexture::value(double u, double v, const glm::dvec3& p) const
{
	auto xInteger = int(std::floor(m_invScale * p.x));
	auto yInteger = int(std::floor(m_invScale * p.y));
	auto zInteger = int(std::floor(m_invScale * p.z));

	bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

	return isEven ? m_even->value(u, v, p) : m_odd->value(u, v, p);
}

ImageTexture::ImageTexture(const char* filePath)
	: m_image(filePath)
{
}

glm::dvec3 ImageTexture::value(double u, double v, const glm::dvec3& pos) const
{
	if (m_image.isNull() || m_image.height() <= 0)
		return glm::dvec3(0, 1, 1);

	u = Interval(0, 1).clamp(u);
	v = 1.0 - Interval(0, 1).clamp(v);

	auto i = int(u * m_image.width());
	auto j = int(v * m_image.height());

	i = std::clamp(i, 0, m_image.width() - 1);
	j = std::clamp(j, 0, m_image.height() - 1);

	QColor pixelColor = m_image.pixel(i, j);
	return glm::dvec3(pixelColor.redF(), pixelColor.greenF(), pixelColor.blueF());
}
