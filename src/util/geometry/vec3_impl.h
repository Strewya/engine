#pragma once
/********************************************
*  contents:   3D vector function implementations
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   template <typename T> void vec3<T>::set(T x, T y, T z)
   {
      this->x = x;
      this->y = y;
      this->z = z;
   }

   template<typename T> bool vec3<T>::isZero() const
   {
      return x == 0.0f && y == 0.0f && z == 0.0f;
   }

   template<typename T> template<typename U> vec3<T>::operator core::vec3<U>() const
   {
      return vec3 < U > {static_cast<U>(x), static_cast<U>(y)};
   }


   template<typename T> vec3<T>& operator+=(vec3<T>& v, T s)
   {
      v.x += s;
      v.y += s;
      v.z += s;
      return v;
   }

   template<typename T> vec3<T>& operator-=(vec3<T>& v, T s)
   {
      v.x -= s;
      v.y -= s;
      v.z -= s;
      return v;
   }

   template<typename T> vec3<T>& operator*=(vec3<T>& v, T s)
   {
      v.x *= s;
      v.y *= s;
      v.z *= s;
      return v;
   }

   template<typename T> vec3<T>& operator/=(vec3<T>& v, T s)
   {
      v.x /= s;
      v.y /= s;
      v.z /= s;
      return v;
   }

   template<typename T> vec3<T>& operator+=(vec3<T>& l, vec3<T> r)
   {
      l.x += r.x;
      l.y += r.y;
      l.z += r.z;
      return l;
   }

   template<typename T> vec3<T>& operator-=(vec3<T>& l, vec3<T> r)
   {
      l.x -= r.x;
      l.y -= r.y;
      l.z -= r.z;
      return l;
   }

   template<typename T> vec3<T>& operator*=(vec3<T>& l, vec3<T> r)
   {
      l.x *= r.x;
      l.y *= r.y;
      l.z *= r.z;
      return l;
   }

   template<typename T> vec3<T>& operator/=(vec3<T>& l, vec3<T> r)
   {
      l.x /= r.x;
      l.y /= r.y;
      l.z /= r.z;
      return l;
   }

   template<typename T> T vec3<T>::length(vec3<T> v)
   {
      return static_cast<T>(sqrt(static_cast<float>(length2(v))));
   }

   template<typename T> T vec3<T>::length2(vec3<T> v)
   {
      return v.x*v.x + v.y*v.y + v.z*v.z;
   }

   template<typename T> vec3<T> vec3<T>::normalize(vec3<T> v)
   {
      auto l2 = vec3::length2(v);
      if( l2 == 0 )
         return{0, 0, 0};
      auto l = static_cast<T>(sqrt(static_cast<float>(l2)));
      return{v.x / l, v.y / l, v.z / l};
   }

   template<typename T> T vec3<T>::dotProduct(vec3<T> l, vec3<T> r)
   {
      return l.x*r.x + l.y*r.y + l.z*r.z;
   }



   template<typename T> vec3<T> operator-(vec3<T> v)
   {
      return{-v.x, -v.y, -v.z};
   }

   template<typename T> vec3<T> operator+(vec3<T> v, T s)
   {
      v += s;
      return v;
   }
   template<typename T> vec3<T> operator-(vec3<T> v, T s)
   {
      v -= s;
      return v;
   }
   template<typename T> vec3<T> operator*(vec3<T> v, T s)
   {
      v *= s;
      return v;
   }
   template<typename T> vec3<T> operator/(vec3<T> v, T s)
   {
      v /= s;
      return v;
   }

   template<typename T> vec3<T> operator+(vec3<T> l, vec3<T> r)
   {
      l += r;
      return l;
   }
   template<typename T> vec3<T> operator-(vec3<T> l, vec3<T> r)
   {
      l -= r;
      return l;
   }
   template<typename T> vec3<T> operator*(vec3<T> l, vec3<T> r)
   {
      l *= r;
      return l;
   }
   template<typename T> vec3<T> operator/(vec3<T> l, vec3<T> r)
   {
      l /= r;
      return l;
   }

   template<typename T> bool operator==(vec3<T> l, vec3<T> r)
   {
      return l.x == r.x && l.y == r.y && l.z == r.z;
   }
   template<typename T> bool operator!=(vec3<T> l, vec3<T> r)
   {
      return !(l == r);
   }

   template<typename T> std::ostream& operator<<(std::ostream& os, vec3<T> v)
   {
      os << "{" << v.x << "," << v.y << "," << v.z << "}";
      return os;
   }
}
