#pragma once
/********************************************
*  contents:   2D vector declaration
*  usage:
********************************************/
/******* c++ headers *******/
#include <ostream>
/******* common headers *******/
/******* extra headers *******/
#include "util/geometry/vec2_fwd.h"
/******* end header inclusion *******/

namespace Core
{
   template<typename T>
   class Vec2
   {
   public:
      T x;
      T y;

      void set(T x, T y);

      template<typename U> operator Vec2<U>() const;

      static T length(Vec2 v);
      static T length2(Vec2 v);
      static Vec2 normalize(Vec2 v);
      static T dotProduct(Vec2 l, Vec2 r);
      static Vec2 projection(Vec2 v, Vec2 target);
   };

   template<typename T> Vec2<T> operator-(Vec2<T> v);

   template<typename T> Vec2<T>& operator+=(Vec2<T>& t, T s);
   template<typename T> Vec2<T>& operator-=(Vec2<T>& t, T s);
   template<typename T> Vec2<T>& operator*=(Vec2<T>& t, T s);
   template<typename T> Vec2<T>& operator/=(Vec2<T>& t, T s);
   
   template<typename T> Vec2<T>& operator+=(Vec2<T>& t, Vec2<T> s);
   template<typename T> Vec2<T>& operator-=(Vec2<T>& t, Vec2<T> s);
   template<typename T> Vec2<T>& operator*=(Vec2<T>& t, Vec2<T> s);
   template<typename T> Vec2<T>& operator/=(Vec2<T>& t, Vec2<T> s);

   template<typename T> Vec2<T> operator+(Vec2<T> v, T s);
   template<typename T> Vec2<T> operator-(Vec2<T> v, T s);
   template<typename T> Vec2<T> operator*(Vec2<T> v, T s);
   template<typename T> Vec2<T> operator/(Vec2<T> v, T s);

   template<typename T> Vec2<T> operator+(Vec2<T> l, Vec2<T> r);
   template<typename T> Vec2<T> operator-(Vec2<T> l, Vec2<T> r);
   template<typename T> Vec2<T> operator*(Vec2<T> l, Vec2<T> r);
   template<typename T> Vec2<T> operator/(Vec2<T> l, Vec2<T> r);

   template<typename T> bool operator==(Vec2<T> l, Vec2<T> r);
   template<typename T> bool operator!=(Vec2<T> l, Vec2<T> r);

   template<typename T> std::ostream& operator<<(std::ostream& os, Vec2<T> v);
}

#include <Util/geometry/vec2_impl.h>


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
