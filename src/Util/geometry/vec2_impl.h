#pragma once
/********************************************
*  contents:   2D vector function implementations
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   template <typename T> void Vec2<T>::set(T x, T y)
   {
      this->x = x;
      this->y = y;
   }

   template <typename T> bool Vec2<T>::isZero() const
   {
      return x == 0.0f && y == 0.0f;
   }

   template<typename T> template<typename U> Vec2<T>::operator Core::Vec2<U>() const
   {
      return Vec2 < U > {static_cast<U>(x), static_cast<U>(y)};
   }


   template<typename T> T Vec2<T>::length(Vec2<T> v)
   {
      return static_cast<T>(sqrt(static_cast<float>(length2(v))));
   }

   template<typename T> T Vec2<T>::length2(Vec2<T> v)
   {
      return v.x*v.x + v.y*v.y;
   }

   template<typename T> Vec2<T> Vec2<T>::normalize(Vec2<T> v)
   {
      auto l2 = Vec2::length2(v);
      if( l2 == 0 )
         return{0, 0};
      auto l = static_cast<T>(sqrt(static_cast<float>(l2)));
      return{v.x / l, v.y / l};
   }

   template<typename T> T Vec2<T>::dotProduct(Vec2<T> l, Vec2<T> r)
   {
      return l.x*r.x + l.y*r.y;
   }

   template<typename T> Vec2<T> Vec2<T>::projection(Vec2<T> vec, Vec2<T> target)
   {
      auto dpv = Vec2::dotProduct(vec, target);
      auto dpt = Vec2::dotProduct(target, target);
      return (dpv / dpt)*target;
   }


   template<typename T> Vec2<T>& operator+=(Vec2<T>& t, T s)
   {
      t.x += s;
      t.y += s;
      return t;
   }

   template<typename T> Vec2<T>& operator-=(Vec2<T>& t, T s)
   {
      t.x -= s;
      t.y -= s;
      return t;
   }

   template<typename T> Vec2<T>& operator*=(Vec2<T>& t, T s)
   {
      t.x *= s;
      t.y *= s;
      return t;
   }

   template<typename T> Vec2<T>& operator/=(Vec2<T>& t, T s)
   {
      t.x /= s;
      t.y /= s;
      return t;
   }

   template<typename T> Vec2<T>& operator+=(Vec2<T>& t, Vec2<T> v)
   {
      t.x += v.x;
      t.y += v.y;
      return t;
   }

   template<typename T> Vec2<T>& operator-=(Vec2<T>& t, Vec2<T> v)
   {
      t.x -= v.x;
      t.y -= v.y;
      return t;
   }

   template<typename T> Vec2<T>& operator*=(Vec2<T>& t, Vec2<T> v)
   {
      t.x *= v.x;
      t.y *= v.y;
      return t;
   }

   template<typename T> Vec2<T>& operator/=(Vec2<T>& t, Vec2<T> v)
   {
      t.x /= v.x;
      t.y /= v.y;
      return t;
   }

   template<typename T> Vec2<T> operator-(Vec2<T> v)
   {
      return{-v.x, -v.y};
   }

   template<typename T> Vec2<T> operator+(Vec2<T> v, T s)
   {
      v += s;
      return v;
   }
   template<typename T> Vec2<T> operator-(Vec2<T> v, T s)
   {
      v -= s;
      return v;
   }
   template<typename T> Vec2<T> operator*(Vec2<T> v, T s)
   {
      v *= s;
      return v;
   }
   template<typename T> Vec2<T> operator/(Vec2<T> v, T s)
   {
      v /= s;
      return v;
   }
   template<typename T> Vec2<T> operator*(T s, Vec2<T> v)
   {
      return v*t;
   }
   template<typename T> Vec2<T> operator/(T s, Vec2<T> v)
   {
      return v/t;
   }

   template<typename T> Vec2<T> operator+(Vec2<T> l, Vec2<T> r)
   {
      l += r;
      return l;
   }
   template<typename T> Vec2<T> operator-(Vec2<T> l, Vec2<T> r)
   {
      l -= r;
      return l;
   }
   template<typename T> Vec2<T> operator*(Vec2<T> l, Vec2<T> r)
   {
      l *= r;
      return l;
   }
   template<typename T> Vec2<T> operator/(Vec2<T> l, Vec2<T> r)
   {
      l /= r;
      return l;
   }

   template<typename T> bool operator==(Vec2<T> l, Vec2<T> r)
   {
      return l.x == r.x && l.y == r.y;
   }
   template<typename T> bool operator!=(Vec2<T> l, Vec2<T> r)
   {
      return !(l == r);
   }

   template<typename T> std::ostream& operator<<(std::ostream& os, Vec2<T> v)
   {
      os << "{" << v.x << "," << v.y << "}";
      return os;
   }
}
