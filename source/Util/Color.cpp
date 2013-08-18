//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Util/Color.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Util
{
	Color::Color()
		: r()
	{
	}

	Color::Color(const Color &rhs)
		: r(rhs.r), g(rhs.g), b(rhs.b), a(rhs.a)
	{
	}

	Color::Color(float r, float g, float b, float a)
		: r(r), g(g), b(b), a(a)
	{
	}

	Color::Color(const float* RGBA)
		: r(RGBA[0]), g(RGBA[1]), b(RGBA[2]), a(RGBA[3])
	{
	}

	Color::~Color() {};

	Color& Color::operator=(const Color &rhs)
	{
		return set(rhs);
	}

	bool Color::operator==(const Color& rhs) const
	{
		if((uint32_t)(r*255) != (uint32_t)(rhs.r*255)) return false;
		if((uint32_t)(g*255) != (uint32_t)(rhs.g*255)) return false;
		if((uint32_t)(b*255) != (uint32_t)(rhs.b*255)) return false;
		if((uint32_t)(a*255) != (uint32_t)(rhs.a*255)) return false;
		return true;
	}

	Color& Color::set(const Color &rhs)
	{
		r = rhs.r;
		g = rhs.g;
		b = rhs.b;
		a = rhs.a;
		return *this;
	}

	void Color::setRed(float r)
	{
		this->r = r;
	}

	void Color::setGreen(float g)
	{
		this->g = g;
	}

	void Color::setBlue(float b)
	{
		this->b = b;
	}

	void Color::setAlpha(float a)
	{
		this->a = a;
	}

	void Color::setRGBA(float r, float g, float b, float a)
	{
		setRed(r);
		setGreen(g);
		setBlue(b);
		setAlpha(a);
	}

	float Color::getAlpha() const
	{
		return a;
	}

	float Color::getRed() const
	{
		return r;
	}

	float Color::getGreen() const
	{
		return g;
	}

	float Color::getBlue() const
	{
		return b;
	}

	const float *const Color::getRGBA() const
	{
		return &r;
	}
}

std::istream& operator>>(std::istream& ss, Util::Color& c)
{
	float r, g, b, a;
	ss >> r >> g >> b >> a;
	c.setRGBA(r,g,b,a);
	return ss;
}

std::ostream& operator<<(std::ostream& ss, Util::Color& c)
{
	ss  << c.getRed() << " " << c.getGreen() << " " << c.getBlue() << " " << c.getAlpha();
	return ss;
}