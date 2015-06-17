//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "util/collision_checks.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "util/utility.h"
#include "util/geometry/circle.h"
#include "util/geometry/rect.h"
#include "util/geometry/vec2.h"
/******* end headers *******/

namespace core
{
   CollisionResult checkCollision(Vec2 point, Rect rect)
   {
      auto toLeft = point.x - rect.left();
      auto toRight = rect.right() - point.x;
      auto toTop = rect.top() - point.y;
      auto toBottom = point.y - rect.bottom();

      CollisionResult result{};
      result.isColliding = (toLeft > 0 && toRight > 0 && toTop > 0 && toBottom > 0);
      result.displacement = {min(toLeft, toRight), min(toTop, toBottom)};

      if( abs(result.displacement.x) < abs(result.displacement.y) )
      {
         result.displacement.y = 0;
      }
      else
      {
         result.displacement.x = 0;
      }
      result.displacement *= vec2::normalize((point - rect.center)*vec2::normalize(result.displacement));

      return result;
   }

   CollisionResult checkCollision(Rect rect, Vec2 point)
   {
      auto result = checkCollision(point, rect);
      return result;
   }

   CollisionResult checkCollision(Vec2 point, Circle circle)
   {
      auto direction = point - circle.center;
      auto l = vec2::length(direction);
      auto r = circle.radius;
      
      CollisionResult result{};
      result.isColliding = (l <= r);
      result.displacement = vec2::normalize(direction)*(r - l);

      return result;
   }

   CollisionResult checkCollision(Circle circle, Vec2 point)
   {
      auto result = checkCollision(point, circle);
      return result;
   }

   CollisionResult checkCollision(Circle circle, Rect rect)
   {
      auto V = circle.center - rect.center;
      clamp(V.x, -rect.halfSize.x, rect.halfSize.x);
      clamp(V.y, -rect.halfSize.y, rect.halfSize.y);
      auto Bc = rect.center + V;
      auto D = circle.center - Bc;
      auto Dlen = vec2::length(D);
      
      CollisionResult result{};
      result.isColliding = (circle.radius >= Dlen);
      result.displacement = vec2::normalize(D)*(circle.radius - Dlen);

      return result;
   }

   CollisionResult checkCollision(Rect rect, Circle circle)
   {
      auto result = checkCollision(circle, rect);
      result.displacement = -result.displacement;
      return result;
   }

   CollisionResult checkCollision(Vec2 a, Vec2 b)
   {
      CollisionResult result{};
      result.isColliding = (a == b);
      result.displacement = {};
      return result;
   }

   CollisionResult checkCollision(Circle a, Circle b)
   {
      b.radius += a.radius;

      auto result = checkCollision(a.center, b);
      return result;
   }

   CollisionResult checkCollision(Rect a, Rect b)
   {
      b.halfSize += a.halfSize;

      auto result = checkCollision(a.center, b);
      return result;
   }






   bool isFullyWithin(Vec2 point, Rect rect)
   {
      auto result = true;
      if( point.x <= rect.left() ) result = false;
      else if( point.x >= rect.right() ) result = false;
      else if( point.y <= rect.bottom() ) result = false;
      else if( point.y >= rect.top() ) result = false;
      return result;
   }

   bool isFullyWithin(Vec2 point, Circle circle)
   {
      auto result = (vec2::length2(point - circle.center) < circle.radius*circle.radius);
      return result;
   }

   bool isFullyWithin(Rect inner, Rect outer)
   {
      outer.halfSize -= inner.halfSize;
      auto result = isFullyWithin(inner.center, outer);
      return result;
   }

   bool isFullyWithin(Rect rect, Circle circle)
   {
//      circle.radius -= vec2::length(rect.halfSize);
//      auto result = isFullyWithin(rect.center, circle);
      auto result = true;
      if( !isFullyWithin(Vec2{rect.left(), rect.top()}, circle) ) result = false;
      else if( !isFullyWithin(Vec2{rect.left(), rect.bottom()}, circle) ) result = false;
      else if( !isFullyWithin(Vec2{rect.right(), rect.top()}, circle) ) result = false;
      else if( !isFullyWithin(Vec2{rect.right(), rect.bottom()}, circle) ) result = false;
      return result;
   }

   bool isFullyWithin(Circle inner, Circle outer)
   {
      outer.radius -= inner.radius;
      auto result = isFullyWithin(inner.center, outer);
      return result;
   }

   bool isFullyWithin(Circle circle, Rect rect)
   {
      rect.halfSize -= {circle.radius, circle.radius};
      auto result = isFullyWithin(circle.center, rect);
      return result;
   }

   struct Line
   {
      Vec2 a;
      Vec2 b;
   };

   bool isLineTouchingCircle(Vec2 A, Vec2 B, Circle circle)
   {
      auto result = false;
      A -= circle.center;
      B -= circle.center;
      circle.center = {0, 0};
      auto D = B - A;
      auto a = vec2::length2(D);
      auto b = 2 * vec2::dotProduct(A, D);
      auto c = vec2::length2(A) - circle.radius*circle.radius;
      auto d = b*b - 4 * a*c;
      if( d >= 0 )
      {
         auto t1 = (-b + d) / (2 * a);
         auto t2 = (-b - d) / (2 * a);
         result = (t1 >= 0 && t1 <= 1) || (t2 >= 0 && t2 <= 1);
      }
      return result;
   }
}
