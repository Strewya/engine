//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Util/Color.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Util
{
	Color::Color()
		: red(_red), blue(_blue), green(_green), alpha(_alpha)
	{
		_red = _green = _blue = _alpha = 255;
	}

	Color::Color(const Color &c)
		: red(_red), blue(_blue), green(_green), alpha(_alpha)
	{
		_red = c.red;
		_green = c.green;
		_blue = c.blue;
		_alpha = c.alpha;
	}

	Color::Color(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
		: red(_red), blue(_blue), green(_green), alpha(_alpha)
	{
		setRed(r);
		setGreen(g);
		setBlue(b);
		setAlpha(a);
	}

	Color::~Color() {};

	Color& Color::operator=(const Color &c)
	{
		return Assign(c);
	}

	bool Color::operator==(const Color& rhs)
	{
		return ((red == rhs.red) && (green == rhs.green) && (blue == rhs.blue) && (alpha == rhs.alpha));
	}

	Color& Color::Assign(const Color &c)
	{
		if(this != &c)
		{
			setRed(c.red);
			setGreen(c.green);
			setBlue(c.blue);
			setAlpha(c.alpha);
		}
		return *this;
	}

	void Color::setRed(uint32_t r)
	{
		_red = r > 255 ? 255 : ( r < 0 ? 0 : r );
	}

	void Color::setGreen(uint32_t g)
	{
		_green = g > 255 ? 255 : ( g < 0 ? 0 : g );
	}

	void Color::setBlue(uint32_t b)
	{
		_blue = b > 255 ? 255 : ( b < 0 ? 0 : b );
	}

	void Color::setAlpha(uint32_t a)
	{
		_alpha = a > 255 ? 255 : ( a < 0 ? 0 : a);
	}

	void Color::setChannels(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
	{
		_red = r > 255 ? 255 : ( r < 0 ? 0 : r );
		_green = g > 255 ? 255 : ( g < 0 ? 0 : g );
		_blue = b > 255 ? 255 : ( b < 0 ? 0 : b );
		_alpha = a > 255 ? 255 : ( a < 0 ? 0 : a);
	}
}

std::istream& operator>>(std::istream& ss, Util::Color& c)
{
	uint32_t r, g, b, a;
	ss >> r >> g >> b >> a;
	c.setAlpha(a);
	c.setBlue(b);
	c.setGreen(g);
	c.setRed(r);
	return ss;
}

std::ostream& operator<<(std::ostream& ss, Util::Color& c)
{
	ss  << c.red << " " << c.green << " " << c.blue << " " << c.alpha;
	return ss;
}