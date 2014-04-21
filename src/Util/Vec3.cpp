//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Util/Vec3.h>
/******* C++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
	Vec3::Vec3(float x, float y, float z)
		: x(x), y(y), z(z)
	{}

	Vec3::Vec3()
		: Vec3(0, 0, 0)
	{}

	void Vec3::set(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}


	Vec3 Vec3::operator+(const Vec3& v) const
	{
		return Vec3(x + v.x, y + v.y, z + v.z);
	}

	Vec3& Vec3::operator+=(const Vec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vec3 Vec3::operator-() const
	{
		return Vec3(-x, -y, -z);
	}

	Vec3 Vec3::operator-(const Vec3& v) const
	{
		return Vec3(x - v.x, y - v.y, z - v.z);
	}

	Vec3 Vec3::operator*(float f) const
	{
		return Vec3(x * f, y * f, z * f);
	}

	Vec3 Vec3::operator/(float f) const
	{
		return Vec3(x / f, y / f, z / f);
	}

	Vec3 operator*(float f, const Vec3& v)
	{
		return v*f;
	}

	Vec3 operator/(float f, const Vec3& v)
	{
		return v / f;
	}

	std::ostream& operator<<(std::ostream& os, const Vec3& v)
	{
		os << "{" << v.x << "," << v.y  << "," << v.z << "}";
		return os;
	}
}


//
//namespace Util
//{
//	/*************************** VEC3 FUNCTIONS ***************************/
//	Vec3::Vec3(const Vec2& v)
//		:Vec3(v.x, v.y, 0)
//	{}
//
//	Vec3::Vec3(const Vec3 &v)
//		: Vec3(v.x, v.y, v.z)
//	{}
//
//	Vec3::Vec3(float x, float y, float z)
//		: x(x), y(y), z(z)
//	{}
//
//	bool Vec3::operator==(const Vec3& v) const
//	{
//		return (std::abs(x - v.x) < std::numeric_limits<float>::epsilon() &&
//			std::abs(y - v.y) < std::numeric_limits<float>::epsilon() &&
//			std::abs(z - v.z) < std::numeric_limits<float>::epsilon());
//	}
//
//	bool Vec3::operator!=(const Vec3 &v) const
//	{
//		return !operator==(v);
//	}
//
//	Vec3 Vec3::operator*(float v) const
//	{
//		return Vec3(x * v, y * v, z * v);
//	}
//
//	Vec3 Vec3::operator*(const Vec3 &v) const
//	{
//		return Vec3(x * v.x, y * v.y, z * v.z);
//	}
//
//	Vec3 operator*(float f, const Vec3& v)
//	{
//		return v*f;
//	}
//
//	Vec3& Vec3::operator*=(const Vec3 &v)
//	{
//		*this = *this * v;
//		return *this;
//	}
//
//	Vec3& Vec3::operator*=(float v)
//	{
//		*this = *this * v;
//		return *this;
//	}
//
//	Vec3 Vec3::operator+(const Vec3 &v) const
//	{
//		return Vec3(x + v.x, y + v.y, z + v.z);
//	}
//
//	Vec3 Vec3::operator+(float v) const
//	{
//		return Vec3(x + v, y + v, z + v);
//	}
//
//	Vec3 operator+(float f, const Vec3& v)
//	{
//		return v+f;
//	}
//
//	Vec3& Vec3::operator+=(const Vec3 &v)
//	{
//		*this = *this + v;
//		return *this;
//	}
//
//	Vec3& Vec3::operator+=(float v)
//	{
//		*this = *this + v;
//		return *this;
//	}
//
//	Vec3 Vec3::operator-() const
//	{
//		return Vec3(-x, -y, -z);
//	}
//
//	Vec3 Vec3::operator-(const Vec3 &v) const
//	{
//		return Vec3(x - v.x, y - v.y, z - v.z);
//	}
//
//	Vec3 Vec3::operator-(float v) const
//	{
//		return Vec3(x - v, y - v, z - v);
//	}
//
//	Vec3 operator-(float f, const Vec3& v)
//	{
//		return v-f;
//	}
//
//	Vec3& Vec3::operator-=(const Vec3 &v)
//	{
//		*this = *this - v;
//		return *this;
//	}
//
//	Vec3& Vec3::operator-=(float v)
//	{
//		*this = *this - v;
//		return *this;
//	}
//
//	Vec3 Vec3::operator/(const Vec3 &v) const
//	{
//		return Vec3(	v.x == 0 ? 0 : x / v.x, 
//						v.y == 0 ? 0 : y / v.y, 
//						v.z == 0 ? 0 : z / v.z);
//	}
//
//	Vec3 Vec3::operator/(float v) const
//	{
//		if(v == 0)
//			return Vec3(0,0,0);
//		return Vec3(x / v, y / v, z / v);
//	}
//
//	Vec3 operator/(float f, const Vec3& v)
//	{
//		return v/f;
//	}
//
//	Vec3& Vec3::operator/=(const Vec3& v)
//	{
//		*this = *this/v;
//		return *this;
//	}
//
//	Vec3& Vec3::operator/=(float v)
//	{
//		*this = *this/v;
//		return *this;
//	}
//
//	Vec3& Vec3::operator=(const Vec3 &v)
//	{
//		return set(v);
//	}
//
//	Vec2 toVec2(const Vec3& vec)
//	{
//		return Vec2(vec.x, vec.y);
//	}
//
//	Vec3& Vec3::set(const Vec3& v)
//	{
//		if(this != &v)
//		{
//			x = v.x;
//			y = v.y;
//			z = v.z;
//		}
//		return *this;
//	}
//
//	Vec3& Vec3::set(float x, float y, float z)
//	{
//		this->x = x;
//		this->y = y;
//		this->z = z;
//		return *this;
//	}
//
//	float Vec3::Dot(const Vec3& v) const
//	{
//		return x*v.x + y*v.y + z*v.z;
//	}
//
//	Vec3 Vec3::Normalized() const
//	{
//		float length = Length();
//		if(length==0)
//			return Vec3(0,0,0);
//		return Vec3(x/length, y/length, z/length);
//	}
//
//	Vec3 Vec3::Cross(const Vec3& b) const
//	{
//		return Vec3(y*b.z-z*b.y, z*b.x-x*b.z, x*b.y-y*b.x);
//	}
//
//	void Vec3::SetLength(float len)
//	{
//		*this = Normalized() * len;
//	}
//
//	void Vec3::Truncate(float len)
//	{
//		if(LengthSq() > len*len)
//			SetLength(len);
//	}
//
//	float Vec3::Length() const
//	{
//		return sqrtf(LengthSq());
//	}
//
//	float Vec3::LengthSq() const
//	{
//		return x*x + y*y + z*z;
//	}
//
//	float Vec3::DistanceTo(const Vec3& v) const
//	{
//		return (*this - v).Length();
//	}
//
//	float Vec3::SquaredDistanceTo(const Vec3& v) const
//	{
//		return (*this - v).LengthSq();
//	}
//
//	bool Vec3::IsZero() const
//	{
//		return (x==0 && y==0 && z==0);
//	}
//
//	void Vec3::SetZero()
//	{
//		x = y = z = 0;
//	}
//}
