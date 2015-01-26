//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Util/CollisionChecks.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/Circle.h>
#include <Util/Rect.h>
#include <Util/Vec2.h>
/******* end headers *******/

namespace Core
{
   bool isPointInsideRect(Vec2f point, Rect rect)
   {
      if( point.x < rect.center.x - rect.halfWidth ) return false;
      if( point.x > rect.center.x + rect.halfWidth ) return false;
      if( point.y < rect.center.y - rect.halfHeight ) return false;
      if( point.y > rect.center.y + rect.halfHeight ) return false;
      return true;
   }

   bool isPointInsideCircle(Vec2f point, Circle circle)
   {
      return (Vec2f::length2(point - circle.center) <= circle.radius*circle.radius);
   }

   bool isRectInsideRect(Rect inner, Rect outer)
   {
      if( !isPointInsideRect({inner.left(), inner.top()}, outer) ) return false;
      if( !isPointInsideRect({inner.left(), inner.bottom()}, outer) ) return false;
      if( !isPointInsideRect({inner.right(), inner.top()}, outer) ) return false;
      if( !isPointInsideRect({inner.right(), inner.bottom()}, outer) ) return false;
      return true;
   }

   bool isCircleInsideCircle(Circle inner, Circle outer)
   {
      auto radiusDiff = outer.radius - inner.radius;
      return (Vec2f::length2(inner.center - outer.center) <= radiusDiff*radiusDiff);
   }

   bool isRectInsideCircle(Rect rect, Circle circle)
   {
      if( !isPointInsideCircle({rect.left(), rect.top()}, circle) ) return false;
      if( !isPointInsideCircle({rect.left(), rect.bottom()}, circle) ) return false;
      if( !isPointInsideCircle({rect.right(), rect.top()}, circle) ) return false;
      if( !isPointInsideCircle({rect.right(), rect.bottom()}, circle) ) return false;
      return true;
   }

   bool isCircleInsideRect(Circle circle, Rect rect)
   {
      if( !isPointInsideRect(circle.center, rect) ) return false;
      Vec2f pts[4]{circle.center, circle.center, circle.center, circle.center};
      pts[0].x -= circle.radius;
      pts[1].x += circle.radius;
      pts[2].y -= circle.radius;
      pts[3].y += circle.radius;
      for( auto i = 0; i < 4; ++i )
      {
         if( !isPointInsideRect(pts[i], rect) )
         {
            return false;
         }
      }
      return true;
   }

   bool isRectTouchingCircle(Rect rect, Circle circle)
   {
      return isCircleTouchingRect(circle, rect);
   }

   bool isCircleTouchingRect(Circle circle, Rect rect)
   {
      if( circle.center.x + circle.radius < rect.left() ) return false;
      if( circle.center.x - circle.radius > rect.right() ) return false;
      if( circle.center.y + circle.radius < rect.bottom() ) return false;
      if( circle.center.y - circle.radius > rect.top() ) return false;
      return true;
   }

   bool isRectTouchingRect(Rect l, Rect r)
   {
      if( l.left() > r.right() ) return false;
      if( l.right() < r.left() ) return false;
      if( l.top() < r.bottom() ) return false;
      if( l.bottom() > r.top() ) return false;
      return true;
   }

   bool isCircleTouchingCircle(Circle l, Circle r)
   {
      return (Vec2f::length(l.center - r.center) <= (l.radius + r.radius));
   }

   bool isLineTouchingCircle(Vec2f A, Vec2f B, Circle circle)
   {
      A -= circle.center;
      B -= circle.center;
      circle.center.set(0, 0);
      auto D = B - A;
      auto a = Vec2f::length2(D);
      auto b = 2 * Vec2f::dotProduct(A, D);
      auto c = Vec2f::length2(A) - circle.radius*circle.radius;
      auto d = b*b - 4 * a*c;
      if( d >= 0 )
      {
         auto t1 = (-b + d) / (2 * a);
         auto t2 = (-b - d) / (2 * a);
         auto y = (t1 >= 0 && t1 <= 1) || (t2 >= 0 && t2 <= 1);
         return y;
      }
      return false;
   }
}
