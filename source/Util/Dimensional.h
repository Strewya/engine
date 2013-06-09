#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <iostream>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Util
{
	class Vec2
	{
	public:
		Vec2();
		Vec2(const Vec2& v);
		Vec2(const float& x, const float& y);
		~Vec2();
		Vec2& operator*=(const Vec2& v);
		Vec2& operator*=(const float& v);
		Vec2& operator/=(const Vec2& v);
		Vec2& operator/=(const float& v);
		Vec2& operator+=(const Vec2& v);
		Vec2& operator+=(const float& v);
		Vec2& operator-=(const Vec2& v);
		Vec2& operator-=(const float& v);
		Vec2 operator+(const Vec2& v) const;
		Vec2 operator+(const float& v) const;
		Vec2 operator-() const;
		Vec2 operator-(const Vec2& v) const;
		Vec2 operator-(const float& v) const;
		Vec2 operator*(const Vec2& v) const;
		Vec2 operator*(const float& v) const;
		Vec2 operator/(const Vec2& v) const;
		Vec2 operator/(const float& v) const;
		bool operator==(const Vec2& v) const;
		bool operator!=(const Vec2& v) const;
		Vec2& operator=(const Vec2& v);
		Vec2& Assign(const Vec2& r);

		float Dot(const Vec2& v) const;
		Vec2 ProjectOn(const Vec2& target) const;
		Vec2 Normalized() const;
		void SetLength(const float& len);
		void Truncate(const float& len);
		float Length() const;
		float LengthSq() const;
		Vec2 LeftNormal() const;
		Vec2 RightNormal() const;
		float DistanceTo(const Vec2& v) const;
		float SquaredDistanceTo(const Vec2& v) const;
		bool IsZero() const;
		void SetZero();

		float x, y;
	};

	Vec2 operator+(const float& f, const Vec2& v);
	Vec2 operator-(const float& f, const Vec2& v);
	Util::Vec2 operator*(const float& f, const Util::Vec2& v);
	Vec2 operator/(const float& f, const Vec2& v);

	float DotProduct(const Vec2& vec1, const Vec2& vec2);
	Vec2 ProjectVector(const Vec2& vec, const Vec2& target);
	Vec2 Normalize(const Vec2& vec);
	Vec2 LeftNormal(const Vec2& vec);
	Vec2 RightNormal(const Vec2& vec);

	class Vec3
	{
	public:
		Vec3();
		Vec3(const Vec3& v);
		Vec3(const float& x, const float& y);
		Vec3(const float& x, const float& y, const float& z);
		~Vec3();
		Vec3& operator*=(const Vec3& v);
		Vec3& operator*=(const float& v);
		Vec3& operator/=(const Vec3& v);
		Vec3& operator/=(const float& v);
		Vec3& operator+=(const Vec3& v);
		Vec3& operator+=(const float& v);
		Vec3& operator-=(const Vec3& v);
		Vec3& operator-=(const float& v);
		Vec3 operator+(const Vec3& v) const;
		Vec3 operator+(const float& v) const;
		Vec3 operator-() const;
		Vec3 operator-(const Vec3& v) const;
		Vec3 operator-(const float& v) const;
		Vec3 operator*(const Vec3& v) const;
		Vec3 operator*(const float& v) const;
		Vec3 operator/(const Vec3& v) const;
		Vec3 operator/(const float& v) const;
		bool operator==(const Vec3& v) const;
		bool operator!=(const Vec3& v) const;
		Vec3& operator=(const Vec3& v);
		Vec3& Assign(const Vec3& r);

		float Dot(const Vec3& v) const;
		//Vec3 ProjectOn(const Vec3& target) const;
		Vec3 Normalized() const;
		void SetLength(const float& len);
		void Truncate(const float& len);
		float Length() const;
		float LengthSq() const;
		Vec3 Cross(const Vec3& rhs) const;
		float DistanceTo(const Vec3& v) const;
		float SquaredDistanceTo(const Vec3& v) const;
		bool IsZero() const;
		void SetZero();

		float x, y, z;
	};

	Vec3 operator+(const float& f, const Vec3& v);
	Vec3 operator-(const float& f, const Vec3& v);
	Vec3 operator*(const float& f, const Vec3& v);
	Vec3 operator/(const float& f, const Vec3& v);
	
	class Rect
	{
	public:
		Rect();
		Rect(const Rect& r);
		Rect(const Vec2& position, const float& width, const float& height);
		Rect(const float& left, const float& top, const float& width, const float& height);
		Rect(const Vec2& topleft, const Vec2& size);
		~Rect();
		//Rect& operator*=(const Rect& r);
		Rect& operator*=(const float& v);
		//Rect& operator/=(const Rect& r);
		//Rect& operator+=(const Rect& r);
		//Rect& operator-=(const Rect& r);
		//Rect operator+(const Rect& r) const;
		//Rect operator-(const Rect& r) const;
		//Rect operator*(const Rect& r) const;
		Rect operator*(const float& v) const;
		//Rect operator/(const Rect& r) const;
		bool operator==(const Rect& r) const;
		bool operator!=(const Rect& r) const;
		Rect& operator=(const Rect& r);
		Rect& Assign(const Rect& r);

		float GetWidth() const;
		float GetHeight() const;
		Vec2 GetSize() const;
		Vec2 GetCenter() const;

		void SetWidth(const float& w);
		void SetHeight(const float& h);
		void SetSize(const Vec2& s);
		void SetCenter(const Vec2& c);
		
		Rect Intersection(const Rect& r) const;
		bool IsIntersected(const Rect& r) const;
		bool IsIntersected(const Vec2& v) const;

		float Left() const;
		float Right() const;
		float Top() const;
		float Bottom() const;

		bool IsZero() const;
		void SetZero();

		Vec2 position;
		float width, height;
	};
}

std::istream& operator>>(std::istream& ss, Util::Vec2& v);
std::ostream& operator<<(std::ostream& ss, Util::Vec2& v);

std::istream& operator>>(std::istream& ss, Util::Vec3& v);
std::ostream& operator<<(std::ostream& ss, Util::Vec3& v);

std::istream& operator>>(std::istream& ss, Util::Rect& r);
std::ostream& operator<<(std::ostream& ss, Util::Rect& r);