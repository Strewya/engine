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
   template <typename T> void vec2<T>::set(T x, T y)
   {
      this->x = x;
      this->y = y;
   }

   template <typename T> bool vec2<T>::isZero() const
   {
      return x == 0.0f && y == 0.0f;
   }

   template<typename T> template<typename U> vec2<T>::operator Core::vec2<U>() const
   {
      return vec2 < U > {static_cast<U>(x), static_cast<U>(y)};
   }


   template<typename T> T vec2<T>::length(vec2<T> v)
   {
      return static_cast<T>(sqrt(static_cast<float>(length2(v))));
   }

   template<typename T> T vec2<T>::length2(vec2<T> v)
   {
      return v.x*v.x + v.y*v.y;
   }

   template<typename T> vec2<T> vec2<T>::normalize(vec2<T> v)
   {
      auto l2 = vec2::length2(v);
      if( l2 == 0 )
         return{0, 0};
      auto l = static_cast<T>(sqrt(static_cast<float>(l2)));
      return{v.x / l, v.y / l};
   }

   template<typename T> T vec2<T>::dotProduct(vec2<T> l, vec2<T> r)
   {
      return l.x*r.x + l.y*r.y;
   }

   template<typename T> vec2<T> vec2<T>::projection(vec2<T> vec, vec2<T> target)
   {
      auto dpv = vec2::dotProduct(vec, target);
      auto dpt = vec2::dotProduct(target, target);
      return (dpv / dpt)*target;
   }


   template<typename T> vec2<T>& operator+=(vec2<T>& t, T s)
   {
      t.x += s;
      t.y += s;
      return t;
   }

   template<typename T> vec2<T>& operator-=(vec2<T>& t, T s)
   {
      t.x -= s;
      t.y -= s;
      return t;
   }

   template<typename T> vec2<T>& operator*=(vec2<T>& t, T s)
   {
      t.x *= s;
      t.y *= s;
      return t;
   }

   template<typename T> vec2<T>& operator/=(vec2<T>& t, T s)
   {
      t.x /= s;
      t.y /= s;
      return t;
   }

   template<typename T> vec2<T>& operator+=(vec2<T>& t, vec2<T> v)
   {
      t.x += v.x;
      t.y += v.y;
      return t;
   }

   template<typename T> vec2<T>& operator-=(vec2<T>& t, vec2<T> v)
   {
      t.x -= v.x;
      t.y -= v.y;
      return t;
   }

   template<typename T> vec2<T>& operator*=(vec2<T>& t, vec2<T> v)
   {
      t.x *= v.x;
      t.y *= v.y;
      return t;
   }

   template<typename T> vec2<T>& operator/=(vec2<T>& t, vec2<T> v)
   {
      t.x /= v.x;
      t.y /= v.y;
      return t;
   }

   template<typename T> vec2<T> operator-(vec2<T> v)
   {
      return{-v.x, -v.y};
   }

   template<typename T> vec2<T> operator+(vec2<T> v, T s)
   {
      v += s;
      return v;
   }
   template<typename T> vec2<T> operator-(vec2<T> v, T s)
   {
      v -= s;
      return v;
   }
   template<typename T> vec2<T> operator*(vec2<T> v, T s)
   {
      v *= s;
      return v;
   }
   template<typename T> vec2<T> operator/(vec2<T> v, T s)
   {
      v /= s;
      return v;
   }
   template<typename T> vec2<T> operator*(T s, vec2<T> v)
   {
      return v*t;
   }
   template<typename T> vec2<T> operator/(T s, vec2<T> v)
   {
      return v/t;
   }

   template<typename T> vec2<T> operator+(vec2<T> l, vec2<T> r)
   {
      l += r;
      return l;
   }
   template<typename T> vec2<T> operator-(vec2<T> l, vec2<T> r)
   {
      l -= r;
      return l;
   }
   template<typename T> vec2<T> operator*(vec2<T> l, vec2<T> r)
   {
      l *= r;
      return l;
   }
   template<typename T> vec2<T> operator/(vec2<T> l, vec2<T> r)
   {
      l /= r;
      return l;
   }

   template<typename T> bool operator==(vec2<T> l, vec2<T> r)
   {
      return l.x == r.x && l.y == r.y;
   }
   template<typename T> bool operator!=(vec2<T> l, vec2<T> r)
   {
      return !(l == r);
   }

   template<typename T> std::ostream& operator<<(std::ostream& os, vec2<T> v)
   {
      os << "{" << v.x << "," << v.y << "}";
      return os;
   }
}
