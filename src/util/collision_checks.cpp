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
   bool isPointInsideRect(Vec2 point, Rect rect)
   {
      auto result = true;
      if( point.x < rect.center.x - rect.halfSize.x ) result = false;
      else if( point.x > rect.center.x + rect.halfSize.x ) result = false;
      else if( point.y < rect.center.y - rect.halfSize.y ) result = false;
      else if( point.y > rect.center.y + rect.halfSize.y ) result = false;
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
      auto result = true;
      if( circle.center.x + circle.radius < rect.left() ) result = false;
      else if( circle.center.x - circle.radius > rect.right() ) result = false;
      else if( circle.center.y + circle.radius < rect.bottom() ) result = false;
      else if( circle.center.y - circle.radius > rect.top() ) result = false;
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
      auto l = point.x - rect.left();
      auto r = rect.right() - point.x;
      auto t = rect.top() - point.y;
      auto b = point.y - rect.bottom();

      Vec2 result = {min(l, r), min(t, b)};
      if( l < r )
      {
         result.x = -result.x;
      }
      if( b < t )
      {
         result.y = -result.y;
      }

      return result;
   }

   Vec2 getDisplacementPointFromCircle(Vec2 point, Circle circle)
   {
      auto diff = point - circle.center;
      auto distance2 = vec2::length2(diff);
      auto radius2 = circle.radius*circle.radius;

      Vec2 result = vec2::normalize(diff)*sqrt(radius2 - distance2);
      return result;
   }

   Vec2 getDisplacementCircleFromRect(Circle circle, Rect rect)
   {
      rect.halfSize.x += circle.radius;
      rect.halfSize.y += circle.radius;
      auto result = getDisplacementPointFromRect(circle.center, rect);
      return result;
   }

   Vec2 getDisplacementCircleFromCircle(Circle l, Circle r)
   {
      l.radius += r.radius;
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
