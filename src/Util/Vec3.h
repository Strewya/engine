#pragma once
/********************************************
*	class:	Vec3
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core 
{
	class Vec3
	{
	public:
		float x, y, z;

		Vec3();
		Vec3(float x, float y, float z);

		void set(float x, float y, float z);

		Vec3 operator+(const Vec3& v) const;
		Vec3& operator+=(const Vec3& v);
		Vec3 operator-() const;
		Vec3 operator-(const Vec3& v) const;
		Vec3 operator*(float f) const;
		Vec3 operator/(float f) const;
	};

	Vec3 operator*(float f, const Vec3& v);
	Vec3 operator/(float f, const Vec3& v);
	std::ostream& operator<<(std::ostream& os, const Vec3& v);
}

// #include <iostream>
//namespace Util
//{
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
//	std::istream& operator>>(std::istream& ss, Util::Vec3& v);
//	std::ostream& operator<<(std::ostream& ss, Util::Vec3& v);
//}
