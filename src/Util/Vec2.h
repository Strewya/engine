#pragma once
/********************************************
*	class:	Vec2
*	usage:
********************************************/
/******* C++ headers *******/
#include <ostream>
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

		/* these are inside the class for tolua++ binding compatibility */
		Vec2 operator+(const Vec2& v) const;
		Vec2& operator+=(const Vec2& v);
		Vec2 operator-() const;
		Vec2 operator-(const Vec2& v) const;
		Vec2 operator*(float f) const;
		Vec2& operator*=(const Vec2& v);
		Vec2& operator*=(float f);
		Vec2 operator/(float f) const;
		Vec2& operator/=(float f);
		
		static float length(const Vec2& v);
		static Vec2 normalize(const Vec2& v);
	};

	Vec2 operator*(float f, const Vec2& v);
	Vec2 operator*(const Vec2& l, const Vec2& r);
	Vec2 operator/(float f, const Vec2& v);
	std::ostream& operator<<(std::ostream& os, const Vec2& v);
}

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
