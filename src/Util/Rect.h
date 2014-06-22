#pragma once
/********************************************
*	class:	Vec2
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
	class Rect
	{
	public:
		Rect();

		float left() const;
		float right() const;
		float top() const;
		float bottom() const;

		Vec2 center;
		float halfWidth;
		float halfHeight;
	};
}

//#include <iostream>
//namespace Util
//{
//	class Rect
//	{
//	public:
//		Rect(float left = 0, float top = 0, float width = 0, float height = 0);
//		Rect(const Vec2& position, float width, float height);
//		Rect(const Vec2& topleft, const Vec2& size);
//		Rect(const Rect& r);
//
//		//Rect& operator*=(const Rect& r);
//		Rect& operator*=(float v);
//		//Rect& operator/=(const Rect& r);
//		//Rect& operator+=(const Rect& r);
//		//Rect& operator-=(const Rect& r);
//		//Rect operator+(const Rect& r) const;
//		//Rect operator-(const Rect& r) const;
//		//Rect operator*(const Rect& r) const;
//		Rect operator*(float v) const;
//		//Rect operator/(const Rect& r) const;
//		bool operator==(const Rect& r) const;
//		bool operator!=(const Rect& r) const;
//		Rect& operator=(const Rect& r);
//		
//		Rect& set(const Rect& r);
//		Rect& set(float x, float y, float w, float h);
//
//		float GetWidth() const;
//		float GetHeight() const;
//		Vec2 GetSize() const;
//		Vec2 GetCenter() const;
//
//		void SetWidth(float w);
//		void SetHeight(float h);
//		void SetSize(const Vec2& s);
//		void SetCenter(const Vec2& c);
//		
//		Rect Intersection(const Rect& r) const;
//		bool IsIntersected(const Rect& r) const;
//		bool IsIntersected(const Vec2& v) const;
//
//		float Left() const;
//		float Right() const;
//		float Top() const;
//		float Bottom() const;
//
//		bool IsZero() const;
//		void SetZero();
//
//		Vec2 position;
//		float width, height;
//	};
//
//	std::istream& operator>>(std::istream& ss, Util::Rect& r);
//	std::ostream& operator<<(std::ostream& ss, Util::Rect& r);
//}
