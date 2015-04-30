#pragma once
/********************************************
*  contents:   3D vector type declaration
*  usage:
********************************************/
/******* c++ headers *******/
#include <ostream>
/******* common headers *******/
/******* extra headers *******/
#include "util/vec3_fwd.h"
/******* end header inclusion *******/

namespace Core
{
   template<typename T>
   class Vec3
   {
   public:
      T x;
      T y;
      T z;

      Vec3();
      Vec3(T x, T y, T z);

      void set(T x, T y, T z);

      template<typename U> operator Vec3<U>() const;

      Vec3& operator+=(T s);
      Vec3& operator+=(Vec3 v);

      Vec3& operator-=(T s);
      Vec3& operator-=(Vec3 v);

      Vec3& operator*=(T s);
      Vec3& operator*=(Vec3 v);

      Vec3& operator/=(T s);
      Vec3& operator/=(Vec3 v);

      static T length(Vec3 v);
      static T length2(Vec3 v);
      static Vec3 normalize(Vec3 v);
      static T dotProduct(Vec3 l, Vec3 r);
   };

   template<typename T> Vec3<T> operator-(Vec3<T> v);

   template<typename T> Vec3<T> operator+(Vec3<T> v, T s);
   template<typename T> Vec3<T> operator-(Vec3<T> v, T s);
   template<typename T> Vec3<T> operator*(Vec3<T> v, T s);
   template<typename T> Vec3<T> operator/(Vec3<T> v, T s);

   template<typename T> Vec3<T> operator+(Vec3<T> l, Vec3<T> r);
   template<typename T> Vec3<T> operator-(Vec3<T> l, Vec3<T> r);
   template<typename T> Vec3<T> operator*(Vec3<T> l, Vec3<T> r);
   template<typename T> Vec3<T> operator/(Vec3<T> l, Vec3<T> r);

   template<typename T> bool operator==(Vec3<T> l, Vec3<T> r);
   template<typename T> bool operator!=(Vec3<T> l, Vec3<T> r);

   template<typename T> std::ostream& operator<<(std::ostream& os, Vec3<T> v);
}

#include <Util/vec3_impl.h>


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