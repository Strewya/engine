#pragma once
/********************************************
*  contents:   3D vector function implementations
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   template <typename T> void Vec3<T>::set(T x, T y, T z)
   {
      this->x = x;
      this->y = y;
      this->z = z;
   }

   template<typename T> bool Vec3<T>::isZero() const
   {
      return x == 0.0f && y == 0.0f && z == 0.0f;
   }

   template<typename T> template<typename U> Vec3<T>::operator Core::Vec3<U>() const
   {
      return Vec3 < U > {static_cast<U>(x), static_cast<U>(y)};
   }


   template<typename T> Vec3<T>& operator+=(Vec3<T>& v, T s)
   {
      v.x += s;
      v.y += s;
      v.z += s;
      return v;
   }

   template<typename T> Vec3<T>& operator-=(Vec3<T>& v, T s)
   {
      v.x -= s;
      v.y -= s;
      v.z -= s;
      return v;
   }

   template<typename T> Vec3<T>& operator*=(Vec3<T>& v, T s)
   {
      v.x *= s;
      v.y *= s;
      v.z *= s;
      return v;
   }

   template<typename T> Vec3<T>& operator/=(Vec3<T>& v, T s)
   {
      v.x /= s;
      v.y /= s;
      v.z /= s;
      return v;
   }

   template<typename T> Vec3<T>& operator+=(Vec3<T>& l, Vec3<T> r)
   {
      l.x += r.x;
      l.y += r.y;
      l.z += r.z;
      return l;
   }

   template<typename T> Vec3<T>& operator-=(Vec3<T>& l, Vec3<T> r)
   {
      l.x -= r.x;
      l.y -= r.y;
      l.z -= r.z;
      return l;
   }

   template<typename T> Vec3<T>& operator*=(Vec3<T>& l, Vec3<T> r)
   {
      l.x *= r.x;
      l.y *= r.y;
      l.z *= r.z;
      return l;
   }

   template<typename T> Vec3<T>& operator/=(Vec3<T>& l, Vec3<T> r)
   {
      l.x /= r.x;
      l.y /= r.y;
      l.z /= r.z;
      return l;
   }

   template<typename T> T Vec3<T>::length(Vec3<T> v)
   {
      return static_cast<T>(sqrt(static_cast<float>(length2(v))));
   }

   template<typename T> T Vec3<T>::length2(Vec3<T> v)
   {
      return v.x*v.x + v.y*v.y + v.z*v.z;
   }

   template<typename T> Vec3<T> Vec3<T>::normalize(Vec3<T> v)
   {
      auto l2 = Vec3::length2(v);
      if( l2 == 0 )
         return{0, 0, 0};
      auto l = static_cast<T>(sqrt(static_cast<float>(l2)));
      return{v.x / l, v.y / l, v.z / l};
   }

   template<typename T> T Vec3<T>::dotProduct(Vec3<T> l, Vec3<T> r)
   {
      return l.x*r.x + l.y*r.y + l.z*r.z;
   }



   template<typename T> Vec3<T> operator-(Vec3<T> v)
   {
      return{-v.x, -v.y, -v.z};
   }

   template<typename T> Vec3<T> operator+(Vec3<T> v, T s)
   {
      v += s;
      return v;
   }
   template<typename T> Vec3<T> operator-(Vec3<T> v, T s)
   {
      v -= s;
      return v;
   }
   template<typename T> Vec3<T> operator*(Vec3<T> v, T s)
   {
      v *= s;
      return v;
   }
   template<typename T> Vec3<T> operator/(Vec3<T> v, T s)
   {
      v /= s;
      return v;
   }

   template<typename T> Vec3<T> operator+(Vec3<T> l, Vec3<T> r)
   {
      l += r;
      return l;
   }
   template<typename T> Vec3<T> operator-(Vec3<T> l, Vec3<T> r)
   {
      l -= r;
      return l;
   }
   template<typename T> Vec3<T> operator*(Vec3<T> l, Vec3<T> r)
   {
      l *= r;
      return l;
   }
   template<typename T> Vec3<T> operator/(Vec3<T> l, Vec3<T> r)
   {
      l /= r;
      return l;
   }

   template<typename T> bool operator==(Vec3<T> l, Vec3<T> r)
   {
      return l.x == r.x && l.y == r.y && l.z == r.z;
   }
   template<typename T> bool operator!=(Vec3<T> l, Vec3<T> r)
   {
      return !(l == r);
   }

   template<typename T> std::ostream& operator<<(std::ostream& os, Vec3<T> v)
   {
      os << "{" << v.x << "," << v.y << "," << v.z << "}";
      return os;
   }
}
