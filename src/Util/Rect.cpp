//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Util/Rect.h>
/******* C++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
	Rect::Rect()
		: halfWidth(0), halfHeight(0)
	{}

	float Rect::left() const
	{
		return center.x - halfWidth;
	}

	float Rect::right() const
	{
		return center.x + halfWidth;
	}

	float Rect::top() const
	{
		return center.y + halfHeight;
	}

	float Rect::bottom() const
	{
		return center.y - halfHeight;
	}
}


//
//namespace Util
//{
//	/*************************** RECT FUNCTIONS ***************************/
//	Rect::Rect(float left, float top, float width, float height)
//		: position(left, top), width(width), height(height)
//	{}
//
//	Rect::Rect(const Vec2& pos, float width, float height)
//		: Rect(pos.x, pos.y, width, height)
//	{}
//	
//	Rect::Rect(const Vec2& topleft, const Vec2& size)
//		: Rect(topleft, size.x, size.y)
//	{}
//	
//	Rect::Rect(const Rect &r)
//		: Rect(r.position, r.width, r.height)
//	{}
//	
//	bool Rect::operator==(const Rect& r) const
//	{
//		return position == r.position &&
//			std::abs(width - r.width) < std::numeric_limits<float>::epsilon() &&
//			std::abs(height - r.height) < std::numeric_limits<float>::epsilon();
//	}
//
//	bool Rect::operator!=(const Rect &r) const
//	{
//		return !operator==(r);
//	}
//	
//	Rect Rect::operator*(float v) const
//	{
//		return Rect(position, width*v, height*v);
//	}
//	/*
//	Rect Rect::operator*(const Rect &r) const
//	{
//		return Rect(left * r.left, top * r.top, right * r.right, bottom * r.bottom);
//	}
//
//	Rect& Rect::operator*=(const Rect &r)
//	{
//		left *= r.left;
//		top *= r.top;
//		right *= r.right;
//		bottom *= r.bottom;
//		return *this;
//	}
//	*/
//	Rect& Rect::operator*=(float v)
//	{
//		width *= v;
//		height *= v;
//		return *this;
//	}
///*
//	Rect Rect::operator+(const Rect &r) const
//	{
//		return Rect(left + r.left, top + r.top, right + r.right, bottom + r.bottom);
//	}
//
//	Rect& Rect::operator+=(const Rect &r)
//	{
//		left += r.left;
//		top += r.top;
//		right += r.right;
//		bottom += r.bottom;
//		return *this;
//	}
//
//	Rect Rect::operator-(const Rect &r) const
//	{
//		return Rect(left - r.left, top - r.top, right - r.right, bottom - r.bottom);
//	}
//
//	Rect& Rect::operator-=(const Rect &r)
//	{
//		left -= r.left;
//		top -= r.top;
//		right -= r.right;
//		bottom -= r.bottom;
//		return *this;
//	}
//
//	Rect Rect::operator/(const Rect &r) const
//	{
//		return Rect(left / r.left, top / r.top, right / r.right, bottom / r.bottom);
//	}
//
//	Rect& Rect::operator/=(const Rect &r)
//	{
//		left /= r.left;
//		top /= r.top;
//		right /= r.right;
//		bottom /= r.bottom;
//		return *this;
//	}
//	*/
//	Rect& Rect::operator=(const Rect &r)
//	{
//		return set(r);
//	}
//
//	Rect& Rect::set(const Rect& r)
//	{
//		if(this != &r)
//		{
//			position = r.position;
//			width = r.width;
//			height = r.height;
//		}
//		return *this;
//	}
//
//	Rect& Rect::set(float x, float y, float w, float h)
//	{
//		position.set(x, y);
//		width = w;
//		height = h;
//		return *this;
//	}
//
//	bool Rect::IsIntersected(const Rect &r) const
//	{
//		Vec2 diff = (r.position - position);
//		return (std::abs(diff.x) < (r.width+width)) && (std::abs(diff.y) < (r.height+height));
//	}
//
//	bool Rect::IsIntersected(const Vec2 &v) const
//	{
//		Vec2 diff = (v - position);
//		return (std::abs(diff.x) < width) && (std::abs(diff.y) < height);
//	}
//
//	Rect Rect::Intersection(const Rect &r) const
//	{
//		Rect rect;
//		if( IsIntersected(r) )
//		{
//			rect.set(std::max(position.x, r.position.x), 
//						std::max(position.y, r.position.y),
//						std::min(position.x + width, r.position.x + r.width),
//						std::min(position.y + height, r.position.y + r.height));
//		}
//		return rect;
//	}
//
//	float Rect::GetHeight() const
//	{
//		return height;
//	}
//
//	float Rect::GetWidth() const
//	{
//		return width;
//	}
//
//	Vec2 Rect::GetSize() const
//	{
//		return Vec2(GetWidth(), GetHeight());
//	}
//
//	void Rect::SetWidth(float w)
//	{
//		width = w;
//	}
//
//	void Rect::SetHeight(float h)
//	{
//		height = h;
//	}
//
//	void Rect::SetSize(const Vec2& s)
//	{
//		SetWidth(s.x);
//		SetHeight(s.y);
//	}
//
//	void Rect::SetCenter(const Vec2& c)
//	{
//		position.x = c.x - width*0.5f;
//		position.y = c.y - height*0.5f;
//	}
//
//	Vec2 Rect::GetCenter() const
//	{
//		return Vec2(position.x + width*0.5f, position.y + height*0.5f);
//	}
//
//	float Rect::Left() const
//	{
//		return position.x;
//	}
//
//	float Rect::Top() const
//	{
//		return position.y;
//	}
//
//	float Rect::Right() const
//	{
//		return position.x + width;
//	}
//
//	float Rect::Bottom() const
//	{
//		return position.y + height;
//	}
//
//	bool Rect::IsZero() const
//	{
//		return (position.IsZero() && width==0 && height==0);
//	}
//
//	void Rect::SetZero()
//	{
//		width = height = 0;
//		position.SetZero();
//	}
//
//	std::istream& operator>>(std::istream& ss, Util::Rect& r)
//	{
//		ss >> r.position.x >> r.position.y >> r.width >> r.height;
//		return ss;
//	}
//
//	std::ostream& operator<<(std::ostream& ss, Util::Rect& r)
//	{
//		ss << r.position.x << " " << r.position.y << " " << r.width << " " << r.height;
//		return ss;
//	}
//}
