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

namespace core
{
   template<typename T>
   struct vec2
   {
      T x;
      T y;

      void set(T x, T y);
      bool isZero() const;

      template<typename U> operator vec2<U>() const;

      static T length(vec2 v);
      static T length2(vec2 v);
      static vec2 normalize(vec2 v);
      static T dotProduct(vec2 l, vec2 r);
      static vec2 projection(vec2 v, vec2 target);
   };

   template<typename T> vec2<T> operator-(vec2<T> v);

   template<typename T> vec2<T>& operator+=(vec2<T>& t, T s);
   template<typename T> vec2<T>& operator-=(vec2<T>& t, T s);
   template<typename T> vec2<T>& operator*=(vec2<T>& t, T s);
   template<typename T> vec2<T>& operator/=(vec2<T>& t, T s);
   
   template<typename T> vec2<T>& operator+=(vec2<T>& t, vec2<T> s);
   template<typename T> vec2<T>& operator-=(vec2<T>& t, vec2<T> s);
   template<typename T> vec2<T>& operator*=(vec2<T>& t, vec2<T> s);
   template<typename T> vec2<T>& operator/=(vec2<T>& t, vec2<T> s);

   template<typename T> vec2<T> operator+(vec2<T> v, T s);
   template<typename T> vec2<T> operator-(vec2<T> v, T s);
   template<typename T> vec2<T> operator*(vec2<T> v, T s);
   template<typename T> vec2<T> operator*(T s, vec2<T> v);
   template<typename T> vec2<T> operator/(vec2<T> v, T s);
   template<typename T> vec2<T> operator/(T s, vec2<T> v);

   template<typename T> vec2<T> operator+(vec2<T> l, vec2<T> r);
   template<typename T> vec2<T> operator-(vec2<T> l, vec2<T> r);
   template<typename T> vec2<T> operator*(vec2<T> l, vec2<T> r);
   template<typename T> vec2<T> operator/(vec2<T> l, vec2<T> r);

   template<typename T> bool operator==(vec2<T> l, vec2<T> r);
   template<typename T> bool operator!=(vec2<T> l, vec2<T> r);

   template<typename T> std::ostream& operator<<(std::ostream& os, vec2<T> v);
}

#include "util/geometry/vec2_impl.h"


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
