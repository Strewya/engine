//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Util/Color.h>
/******* C++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
   Color::Color(float r, float g, float b, float a)
      : r(r), g(g), b(b), a(a)
   {}

   Color::Color()
      : Color(1, 1, 1)
   {}

   void Color::set(float r, float g, float b, float a)
   {
      this->r = r;
      this->g = g;
      this->b = b;
      this->a = a;
   }

   void Color::clip(float threshold)
   {
      if( r < threshold ) r = 0;
      if( g < threshold ) g = 0;
      if( b < threshold ) b = 0;
      if( a < threshold ) a = 0;
   }
}

//namespace Util
//{
//	Color::Color()
//		: r(1), g(1), b(1), a(1)
//	{
//	}
//
//	Color::Color(const Color &rhs)
//		: r(rhs.r), g(rhs.g), b(rhs.b), a(rhs.a)
//	{
//	}
//
//	Color::Color(float r, float g, float b, float a)
//		: r(r), g(g), b(b), a(a)
//	{
//	}
//
//	Color::Color(const float* RGBA)
//		: r(RGBA[0]), g(RGBA[1]), b(RGBA[2]), a(RGBA[3])
//	{
//	}
//
//	Color::~Color() {};
//
//	Color& Color::operator=(const Color &rhs)
//	{
//		return set(rhs);
//	}
//
//	bool Color::operator==(const Color& rhs) const
//	{
//		if((uint32_t)(r*255) != (uint32_t)(rhs.r*255)) return false;
//		if((uint32_t)(g*255) != (uint32_t)(rhs.g*255)) return false;
//		if((uint32_t)(b*255) != (uint32_t)(rhs.b*255)) return false;
//		if((uint32_t)(a*255) != (uint32_t)(rhs.a*255)) return false;
//		return true;
//	}
//
//	Color& Color::set(const Color &rhs)
//	{
//		r = rhs.r;
//		g = rhs.g;
//		b = rhs.b;
//		a = rhs.a;
//		return *this;
//	}
//
//	void Color::setRed(float r)
//	{
//		this->r = r;
//	}
//
//	void Color::setGreen(float g)
//	{
//		this->g = g;
//	}
//
//	void Color::setBlue(float b)
//	{
//		this->b = b;
//	}
//
//	void Color::setAlpha(float a)
//	{
//		this->a = a;
//	}
//
//	void Color::setRGBA(float r, float g, float b, float a)
//	{
//		setRed(r);
//		setGreen(g);
//		setBlue(b);
//		setAlpha(a);
//	}
//
//	float Color::getAlpha() const
//	{
//		return a;
//	}
//
//	float Color::getRed() const
//	{
//		return r;
//	}
//
//	float Color::getGreen() const
//	{
//		return g;
//	}
//
//	float Color::getBlue() const
//	{
//		return b;
//	}
//
//	const float *const Color::getRGBA() const
//	{
//		return &r;
//	}
//}
