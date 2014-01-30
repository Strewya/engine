//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Util/Dimensional.h>
/******* C++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
	/*************************** VEC2 ***************************/
	
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


	Vec2 operator+(const Vec2& a, const Vec2& b)
	{
		return Vec2(a.x + b.x, a.y + b.y);
	}

	Vec2 operator-(const Vec2& v)
	{
		return Vec2(-v.x, -v.y);
	}

	Vec2 operator*(const Vec2& v, float f)
	{
		return Vec2(v.x * f, v.y * f);
	}

	Vec2 operator*(float f, const Vec2& v)
	{
		return v*f;
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
//
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
//
//
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
//
//	std::istream& operator>>(std::istream& ss, Util::Vec2& v)
//	{
//		ss >> v.x;
//		ss >> v.y;
//		return ss;
//	}
//
//	std::ostream& operator<<(std::ostream& ss, Util::Vec2& v)
//	{
//		ss << v.x << " " << v.y;
//		return ss;
//	}
//
//	std::istream& operator>>(std::istream& ss, Util::Vec3& v)
//	{
//		ss >> v.x;
//		ss >> v.y;
//		ss >> v.z;
//		return ss;
//	}
//
//	std::ostream& operator<<(std::ostream& ss, Util::Vec3& v)
//	{
//		ss << v.x << " " << v.y << " " << v.z;
//		return ss;
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