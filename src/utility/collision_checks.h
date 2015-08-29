#pragma once
/********************************************
*  contents:   #description
*  usage:      
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "utility/geometry/vec2.h"
/******* end header inclusion *******/

namespace core
{
   struct Circle;
   struct Rect;

   struct CollisionResult
   {
      v2 displacement;
      bool isColliding;
   };

   CollisionResult checkCollision(v2 point, Rect rect);
   CollisionResult checkCollision(Rect rect, v2 point);
   CollisionResult checkCollision(v2 point, Circle circle);
   CollisionResult checkCollision(Circle circle, v2 point);
   CollisionResult checkCollision(Rect rect, Circle circle);
   CollisionResult checkCollision(Circle circle, Rect rect);
   CollisionResult checkCollision(v2 a, v2 b);
   CollisionResult checkCollision(Circle a, Circle b);
   CollisionResult checkCollision(Rect a, Rect b);
   template<typename T, typename U> CollisionResult checkCollision(T a, U b)
   {
      return CollisionResult{{}, false};
   }

   bool isFullyWithin(v2 inner, Rect outer);
   bool isFullyWithin(v2 inner, Circle outer);
   bool isFullyWithin(Rect inner, Rect outer);
   bool isFullyWithin(Rect inner, Circle outer);
   bool isFullyWithin(Circle inner, Circle outer);
   bool isFullyWithin(Circle inner, Rect outer);
   template<typename T, typename U> bool isFullyWithin(T a, U b)
   {
      return false;
   }

   bool isLineTouchingCircle(v2 A, v2 B, Circle circle);
}
