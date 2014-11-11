//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Util/Vec2.h>
/******* C++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{	
	Vec2::Vec2(float x, float y)
		: x(x), y(y)
	{}

	Vec2::Vec2()
		: Vec2(0, 0)
	{}

	void Vec2::set(float x, float y)
	{
		this->x = x;
		this->y = y;
	}


	Vec2 Vec2::operator+(float f) const
	{
		return{x + f, y + f};
	}

	Vec2 Vec2::operator+(const Vec2& v) const
	{
		return{x + v.x, y + v.y};
	}

	Vec2& Vec2::operator+=(const Vec2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	Vec2 Vec2::operator-() const
	{
		return{-x, -y};
	}

	Vec2 Vec2::operator-(const Vec2& v) const
	{
		return{x - v.x, y - v.y};
	}

	Vec2 Vec2::operator*(float f) const
	{
		return{x * f, y * f};
	}

	Vec2& Vec2::operator*=(const Vec2& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	Vec2& Vec2::operator*=(float f)
	{
		x *= f;
		y *= f;
		return *this;
	}

	Vec2 Vec2::operator/(float f) const
	{
		return{x / f, y / f};
	}

	Vec2& Vec2::operator /= (float f)
	{
		x /= f;
		y /= f;
		return *this;
	}

	Vec2 operator*(float f, const Vec2& v)
	{
		return v*f;
	}

	Vec2 operator*(const Vec2& l, const Vec2& r)
	{
		return{l.x*r.x, l.y*r.y};
	}

	Vec2 operator/(float f, const Vec2& v)
	{
		return v/f;
	}

	float Vec2::length(const Vec2& v)
	{
		return sqrtf(v.x*v.x + v.y*v.y);
	}

	Vec2 Vec2::normalize(const Vec2& v)
	{
		float length = Vec2::length(v);
		if(length == 0)
			return Vec2(0,0);
		return{v.x / length, v.y / length};
	}

	std::ostream& operator<<(std::ostream& os, const Vec2& v)
	{
		os << "{" << v.x << "," << v.y << "}";
		return os;
	}
}


//
//namespace Util
//{
//	/*************************** VEC2 FUNCTIONS ***************************/
//	Vec2::Vec2(const Vec2 &v)
//		: Vec2(v.x, v.y)
//	{}
//
//	Vec2::Vec2(const Vec3& v)
//		: Vec2(v.x, v.y)
//	{}
//
//	Vec2::Vec2(float x, float y)
//		: x(x), y(y)
//	{}
//
//	bool Vec2::operator==(const Vec2& v) const
//	{
//		return (std::abs(x - v.x) < std::numeric_limits<float>::epsilon() &&
//			std::abs(y - v.y) < std::numeric_limits<float>::epsilon());
//	}
//
//	bool Vec2::operator!=(const Vec2 &v) const
//	{
//		return !operator==(v);
//	}
//
//	Vec2 Vec2::operator*(const Vec2 &v) const
//	{
//		return Vec2(x * v.x, y * v.y);
//	}
//
//	Vec2 operator*(float f, const Vec2& v)
//	{
//		return v*f;
//	}
//
//	Vec2& Vec2::operator*=(const Vec2 &v)
//	{
//		*this = *this * v;
//		return *this;
//	}
//
//	Vec2& Vec2::operator*=(float v)
//	{
//		*this = *this * v;
//		return *this;
//	}
//
//	Vec2 Vec2::operator+(const Vec2 &v) const
//	{
//		return Vec2(x + v.x, y + v.y);
//	}
//
//	Vec2 Vec2::operator+(float v) const
//	{
//		return Vec2(x + v, y + v);
//	}
//
//	Vec2 operator+(float f, const Vec2& v)
//	{
//		return v+f;
//	}
//
//	Vec2& Vec2::operator+=(const Vec2 &v)
//	{
//		*this = *this + v;
//		return *this;
//	}
//
//	Vec2& Vec2::operator+=(float v)
//	{
//		*this = *this + v;
//		return *this;
//	}
//
//	Vec2 Vec2::operator-(const Vec2 &v) const
//	{
//		return Vec2(x - v.x, y - v.y);
//	}
//
//	Vec2 Vec2::operator-(float v) const
//	{
//		return Vec2(x - v, y - v);
//	}
//
//	Vec2 operator-(float f, const Vec2& v)
//	{
//		return v-f;
//	}
//
//	Vec2& Vec2::operator-=(const Vec2 &v)
//	{
//		*this = *this - v;
//		return *this;
//	}
//
//	Vec2& Vec2::operator-=(float v)
//	{
//		*this = *this - v;
//		return *this;
//	}
//
//	Vec2 Vec2::operator/(const Vec2 &v) const
//	{
//		return Vec2(	v.x == 0 ? 0 : x / v.x, 
//						v.y == 0 ? 0 : y / v.y);
//	}
//
//	Vec2 Vec2::operator/(float v) const
//	{
//		if(v == 0)
//			return Vec2(0,0);
//		return Vec2(x / v, y / v);
//	}
//
//	Vec2 operator/(float f, const Vec2& v)
//	{
//		return v/f;
//	}
//
//	Vec2& Vec2::operator/=(const Vec2& v)
//	{
//		*this = *this/v;
//		return *this;
//	}
//
//	Vec2& Vec2::operator/=(float v)
//	{
//		*this = *this/v;
//		return *this;
//	}
//
//	Vec2& Vec2::operator=(const Vec2 &v)
//	{
//		return set(v);
//	}
//
//	Vec3 toVec3(const Vec2& vec)
//	{
//		return Util::Vec3(vec.x, vec.y, 0);
//	}
//
//	Vec2& Vec2::set(const Vec2& v)
//	{
//		if(this != &v)
//		{
//			x = v.x;
//			y = v.y;
//		}
//		return *this;
//	}
//
//	Vec2& Vec2::set(float x, float y)
//	{
//		this->x = x;
//		this->y = y;
//		return *this;
//	}
//
//	float Vec2::Dot(const Vec2& v) const
//	{
//		return x*v.x + y*v.y;
//	}
//
//	Vec2 Vec2::ProjectOn(const Vec2& target) const
//	{
//		float dp = Dot(target);
//		Vec2 proj;
//		proj.x = dp * target.x / target.LengthSq();
//		proj.y = dp * target.y / target.LengthSq();
//		return proj;
//	}
//
//	Vec2 Vec2::Normalized() const
//	{
//		float length = Length();
//		if(length==0)
//			return Vec2(0,0);
//		return Vec2(x/length, y/length);
//	}
//
//	Vec2 Vec2::LeftNormal() const
//	{
//		return Vec2(-y, x);
//	}
//
//	Vec2 Vec2::RightNormal() const
//	{
//		return Vec2(y, -x);
//	}
//
//	void Vec2::SetLength(float len)
//	{
//		*this = Normalized() * len;
//	}
//
//	void Vec2::Truncate(float len)
//	{
//		if(LengthSq() > len*len)
//			SetLength(len);
//	}
//
//	float Vec2::Length() const
//	{
//		return sqrtf(LengthSq());
//	}
//
//	float Vec2::LengthSq() const
//	{
//		return x*x + y*y;
//	}
//
//	float Vec2::DistanceTo(const Vec2& v) const
//	{
//		return (*this - v).Length();
//	}
//
//	float Vec2::SquaredDistanceTo(const Vec2& v) const
//	{
//		return (*this - v).LengthSq();
//	}
//
//	bool Vec2::IsZero() const
//	{
//		return (x==0 && y==0);
//	}
//
//	void Vec2::SetZero()
//	{
//		x = y = 0;
//	}
//
//	float DotProduct(const Vec2& u, const Vec2& v)
//	{
//		return u.x*v.x + u.y*v.y;
//	}
//
//	Vec2 ProjectVec2(const Vec2& vec, const Vec2& target)
//	{
//		float dp = DotProduct(vec, target);
//		float lengthSquared = vec.LengthSq();
//		Vec2 proj;
//		proj.x = dp * target.x / lengthSquared;
//		proj.y = dp * target.y / lengthSquared;
//		return proj;
//	}
//
//	Vec2 Normalize(const Vec2& vec)
//	{
//		return vec/vec.Length();
//	}
//
//	Vec2 LeftNormal(const Vec2& vec)
//	{
//		return Vec2(-vec.y, vec.x);
//	}
//
//	Vec2 RightNormal(const Vec2& vec)
//	{
//		return Vec2(vec.y, -vec.x);
//	}
