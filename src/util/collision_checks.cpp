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









   bool isPointInsideRect(Vec2 point, Rect rect)
   {
      auto result = true;
      if( point.x < rect.left() ) result = false;
      else if( point.x > rect.right() ) result = false;
      else if( point.y < rect.bottom() ) result = false;
      else if( point.y > rect.top() ) result = false;
      return result;
   }

   bool isPointInsideCircle(Vec2 point, Circle circle)
   {
      auto result = (vec2::length2(point - circle.center) <= circle.radius*circle.radius);
      return result;
   }

   bool isRectInsideRect(Rect inner, Rect outer)
   {
      auto result = true;
      if( !isPointInsideRect({inner.left(), inner.top()}, outer) ) result = false;
      else if( !isPointInsideRect({inner.left(), inner.bottom()}, outer) ) result = false;
      else if( !isPointInsideRect({inner.right(), inner.top()}, outer) ) result = false;
      else if( !isPointInsideRect({inner.right(), inner.bottom()}, outer) ) result = false;
      return result;
   }

   bool isCircleInsideCircle(Circle inner, Circle outer)
   {
      auto radiusDiff = outer.radius - inner.radius;
      auto result = (vec2::length2(inner.center - outer.center) <= radiusDiff*radiusDiff);
      return result;
   }

   bool isRectInsideCircle(Rect rect, Circle circle)
   {
      auto result = true;
      if( !isPointInsideCircle({rect.left(), rect.top()}, circle) ) result = false;
      else if( !isPointInsideCircle({rect.left(), rect.bottom()}, circle) ) result = false;
      else if( !isPointInsideCircle({rect.right(), rect.top()}, circle) ) result = false;
      else if( !isPointInsideCircle({rect.right(), rect.bottom()}, circle) ) result = false;
      return result;
   }

   bool isCircleInsideRect(Circle circle, Rect rect)
   {
      auto result = false;
      if( isPointInsideRect(circle.center, rect) )
      {
         result = true;
         Vec2 pts[4]{circle.center, circle.center, circle.center, circle.center};
         pts[0].x -= circle.radius;
         pts[1].x += circle.radius;
         pts[2].y -= circle.radius;
         pts[3].y += circle.radius;
         for( auto i = 0; i < 4; ++i )
         {
            if( !isPointInsideRect(pts[i], rect) )
            {
               result = false;
               break;
            }
         }
      }
      return result;
   }

   bool isRectTouchingCircle(Rect rect, Circle circle)
   {
      auto result = isCircleTouchingRect(circle, rect);
      return result;
   }

   bool isCircleTouchingRect(Circle circle, Rect rect)
   {
      auto V = circle.center - rect.center;
      clamp(V.x, -rect.halfSize.x, rect.halfSize.x);
      clamp(V.y, -rect.halfSize.y, rect.halfSize.y);
      auto Bc = rect.center + V;
      auto D = circle.center - Bc;
      auto result = (circle.radius >= vec2::length(D));
      return result;
   }

   bool isRectTouchingRect(Rect l, Rect r)
   {
      auto result = true;
      if( l.left() > r.right() ) result = false;
      else if( l.right() < r.left() ) result = false;
      else if( l.top() < r.bottom() ) result = false;
      else if( l.bottom() > r.top() ) result = false;
      return result;
   }

   bool isCircleTouchingCircle(Circle l, Circle r)
   {
      auto result = (vec2::length(l.center - r.center) <= (l.radius + r.radius));
      return result;
   }

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

   Vec2 getDisplacementPointFromRect(Vec2 point, Rect rect)
   {
      auto toLeft = point.x - rect.left();
      auto toRight = rect.right() - point.x;
      auto toTop = rect.top() - point.y;
      auto toBottom = point.y - rect.bottom();
      Vec2 result{min(toLeft, toRight), min(toTop, toBottom)};
      if( abs(result.x) < abs(result.y) )
      {
         result.y = 0;
      }
      else
      {
         result.x = 0;
      }
      result *= vec2::normalize((point - rect.center)*vec2::normalize(result));
      
      return result;
   }

   Vec2 getDisplacementPointFromCircle(Vec2 point, Circle circle)
   {
      auto diff = point - circle.center;
      auto distance = vec2::length(diff);
      auto radius = circle.radius;

      Vec2 result{};
      if( distance < radius )
      {
         result = vec2::normalize(diff)*(radius - distance);
      }
      return result;
   }

   Vec2 getDisplacementCircleFromRect(Circle circle, Rect rect)
   {
      auto V = circle.center - rect.center;
      clamp(V.x, -rect.halfSize.x, rect.halfSize.x);
      clamp(V.y, -rect.halfSize.y, rect.halfSize.y);
      auto Bc = rect.center + V;
      auto D = circle.center - Bc;
      auto result = vec2::normalize(D)*(circle.radius - vec2::length(D));
      return result;
   }

   Vec2 getDisplacementCircleFromCircle(Circle l, Circle r)
   {
      r.radius += l.radius;
      auto result = getDisplacementPointFromCircle(l.center, r);
      return result;
   }

   Vec2 getDisplacementRectFromCircle(Rect rect, Circle circle)
   {
      auto result = -getDisplacementCircleFromRect(circle, rect);
      return result;
   }

   Vec2 getDisplacementRectFromRect(Rect l, Rect r)
   {
      r.halfSize += l.halfSize;
      auto result = getDisplacementPointFromRect(l.center, r);
      return result;
   }
}
