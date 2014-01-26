#pragma once
/********************************************
*	class:	Vec2
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class Vec2
	{
	public:
		float x, y;

		Vec2();
		Vec2(float x, float y);
		
		void set(float x, float y);
	};

	Vec2 operator+(const Vec2& a, const Vec2& b);
	Vec2 operator-(const Vec2& rhs);
	Vec2 operator*(const Vec2& v, float f);
	Vec2 operator*(float f, const Vec2& v);
}

// #include <iostream>
//namespace Util
//{
//	
//	class Vec2;
//	class Vec3;
//	class Rect;
//
//	class Vec2
//	{
//	public:
//		Vec2(const Vec2& v);
//		Vec2(const Vec3& v);
//		Vec2(float x = 0, float y = 0);
//		Vec2& operator*=(const Vec2& v);
//		Vec2& operator*=(float v);
//		Vec2& operator/=(const Vec2& v);
//		Vec2& operator/=(float v);
//		Vec2& operator+=(const Vec2& v);
//		Vec2& operator+=(float v);
//		Vec2& operator-=(const Vec2& v);
//		Vec2& operator-=(float v);
//		Vec2 operator+(const Vec2& v) const;
//		Vec2 operator+(float v) const;
//		Vec2 operator-(const Vec2& v) const;
//		Vec2 operator-(float v) const;
//		Vec2 operator*(const Vec2& v) const;
//		Vec2 operator/(const Vec2& v) const;
//		Vec2 operator/(float v) const;
//		bool operator==(const Vec2& v) const;
//		bool operator!=(const Vec2& v) const;
//		Vec2& operator=(const Vec2& v);
//
//		Vec2& set(const Vec2& r);
//		Vec2& set(float x, float y);
//
//		float Dot(const Vec2& v) const;
//		Vec2 ProjectOn(const Vec2& target) const;
//		Vec2 Normalized() const;
//		void SetLength(float len);
//		void Truncate(float len);
//		float Length() const;
//		float LengthSq() const;
//		Vec2 LeftNormal() const;
//		Vec2 RightNormal() const;
//		float DistanceTo(const Vec2& v) const;
//		float SquaredDistanceTo(const Vec2& v) const;
//		bool IsZero() const;
//		void SetZero();
//
//		float x, y;
//	};
//
//	Vec2 operator+(float f, const Vec2& v);
//	Vec2 operator-(float f, const Vec2& v);
//	Util::Vec2 operator*(float f, const Util::Vec2& v);
//	Vec2 operator/(float f, const Vec2& v);
//
//	float DotProduct(const Vec2& vec1, const Vec2& vec2);
//	Vec2 ProjectVector(const Vec2& vec, const Vec2& target);
//	Vec2 Normalize(const Vec2& vec);
//	Vec2 LeftNormal(const Vec2& vec);
//	Vec2 RightNormal(const Vec2& vec);
//
//	class Vec3
//	{
//	public:
//		Vec3(const Vec2& v);
//		Vec3(const Vec3& v);
//		Vec3(float x = 0, float y = 0, float z = 0);
//		Vec3& operator*=(const Vec3& v);
//		Vec3& operator*=(float v);
//		Vec3& operator/=(const Vec3& v);
//		Vec3& operator/=(float v);
//		Vec3& operator+=(const Vec3& v);
//		Vec3& operator+=(float v);
//		Vec3& operator-=(const Vec3& v);
//		Vec3& operator-=(float v);
//		Vec3 operator+(const Vec3& v) const;
//		Vec3 operator+(float v) const;
//		Vec3 operator-() const;
//		Vec3 operator-(const Vec3& v) const;
//		Vec3 operator-(float v) const;
//		Vec3 operator*(const Vec3& v) const;
//		Vec3 operator*(float v) const;
//		Vec3 operator/(const Vec3& v) const;
//		Vec3 operator/(float v) const;
//		bool operator==(const Vec3& v) const;
//		bool operator!=(const Vec3& v) const;
//		Vec3& operator=(const Vec3& v);
//
//		Vec3& set(const Vec3& r);
//		Vec3& set(float x, float y, float z);
//
//		float Dot(const Vec3& v) const;
//		//Vec3 ProjectOn(const Vec3& target) const;
//		Vec3 Normalized() const;
//		void SetLength(float len);
//		void Truncate(float len);
//		float Length() const;
//		float LengthSq() const;
//		Vec3 Cross(const Vec3& rhs) const;
//		float DistanceTo(const Vec3& v) const;
//		float SquaredDistanceTo(const Vec3& v) const;
//		bool IsZero() const;
//		void SetZero();
//
//		float x, y, z;
//	};
//
//	Vec3 operator+(float f, const Vec3& v);
//	Vec3 operator-(float f, const Vec3& v);
//	Vec3 operator*(float f, const Vec3& v);
//	Vec3 operator/(float f, const Vec3& v);
//	Vec3 toVec3(const Vec2& vec);
//	Vec2 toVec2(const Vec3& vec);
//	
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
//	
//	std::istream& operator>>(std::istream& ss, Util::Vec2& v);
//	std::ostream& operator<<(std::ostream& ss, Util::Vec2& v);
//
//	std::istream& operator>>(std::istream& ss, Util::Vec3& v);
//	std::ostream& operator<<(std::ostream& ss, Util::Vec3& v);
//
//	std::istream& operator>>(std::istream& ss, Util::Rect& r);
//	std::ostream& operator<<(std::ostream& ss, Util::Rect& r);
//}
