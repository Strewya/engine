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
		: _colorARGB(0xffffffff)
	{
	}

	Color::Color(const Color &rhs)
		: _colorARGB(rhs._colorARGB)
	{
	}

	Color::Color(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
		: _colorARGB(0xffffffff)
	{
		setRed(r);
		setGreen(g);
		setBlue(b);
		setAlpha(a);
	}

	Color::Color(uint8_t r, uint8_t g, uint8_t b)
		: _colorARGB(0xffffffff)
	{
		setRed(r);
		setGreen(g);
		setBlue(b);
		setAlpha(255);
	}

	Color::Color(uint32_t colorARGB)
		: _colorARGB(colorARGB)
	{
	}

	Color::~Color() {};

	Color& Color::operator=(const Color &rhs)
	{
		return Assign(rhs);
	}

	bool Color::operator==(const Color& rhs) const
	{
		return (_colorARGB == rhs._colorARGB);
	}

	Color& Color::Assign(const Color &rhs)
	{
		_colorARGB = rhs._colorARGB;
		return *this;
	}

	void Color::setRed(uint8_t r)
	{
		_colorARGB = (_colorARGB&0xff00ffff)|(r<<16);
	}

	void Color::setGreen(uint8_t g)
	{
		_colorARGB = (_colorARGB&0xffff00ff)|(g<<8);
	}

	void Color::setBlue(uint8_t b)
	{
		_colorARGB = (_colorARGB&0xffffff00)|(b);
	}

	void Color::setAlpha(uint8_t a)
	{
		_colorARGB = (_colorARGB&0x00ffffff)|(a<<24);
	}

	void Color::setChannels(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		setRed(r);
		setGreen(g);
		setBlue(b);
		setAlpha(a);
	}

	uint8_t Color::getAlpha() const
	{
		return (_colorARGB&0xff000000)>>24;
	}

	uint8_t Color::getRed() const
	{
		return (_colorARGB&0x00ff0000)>>16;
	}

	uint8_t Color::getGreen() const
	{
		return (_colorARGB&0x0000ff00)>>8;
	}

	uint8_t Color::getBlue() const
	{
		return (_colorARGB&0x000000ff);
	}

	uint32_t Color::getARGB() const
	{
		return _colorARGB;
	}
}

std::istream& operator>>(std::istream& ss, Util::Color& c)
{
	uint8_t r, g, b, a;
	ss >> r >> g >> b >> a;
	c.setAlpha(a);
	c.setBlue(b);
	c.setGreen(g);
	c.setRed(r);
	return ss;
}

std::ostream& operator<<(std::ostream& ss, Util::Color& c)
{
	ss  << c.getRed() << " " << c.getGreen() << " " << c.getBlue() << " " << c.getAlpha();
	return ss;
}