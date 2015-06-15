#pragma once
/********************************************
*  contents:   #description
*  usage:      
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include "util/geometry/vec2.h"
/******* end header inclusion *******/

namespace core
{
   struct Circle;
   struct Rect;

   struct CollisionResult
   {
      Vec2 displacement;
      bool isColliding;
   };

   CollisionResult checkCollision(Vec2 point, Rect rect);
   CollisionResult checkCollision(Rect rect, Vec2 point);
   CollisionResult checkCollision(Vec2 point, Circle circle);
   CollisionResult checkCollision(Circle circle, Vec2 point);
   CollisionResult checkCollision(Circle a, Circle b);
   CollisionResult checkCollision(Rect a, Rect b);
   CollisionResult checkCollision(Rect rect, Circle circle);
   CollisionResult checkCollision(Circle circle, Rect rect);




   bool isPointInsideRect(Vec2 point, Rect rect);
   bool isPointInsideCircle(Vec2 point, Circle circle);
   bool isRectInsideRect(Rect inner, Rect outer);
   bool isCircleInsideCircle(Circle inner, Circle outer);
   bool isRectInsideCircle(Rect rect, Circle circle);
   bool isCircleInsideRect(Circle circle, Rect rect);

   bool isRectTouchingCircle(Rect rect, Circle circle);
   bool isCircleTouchingRect(Circle circle, Rect rect);
   bool isRectTouchingRect(Rect l, Rect r);
   bool isCircleTouchingCircle(Circle l, Circle r);
   bool isLineTouchingCircle(Vec2 A, Vec2 B, Circle circle);

   Vec2 getDisplacementPointFromRect(Vec2 point, Rect rect);
   Vec2 getDisplacementPointFromCircle(Vec2 point, Circle circle);
   Vec2 getDisplacementCircleFromRect(Circle circle, Rect rect);
   Vec2 getDisplacementCircleFromCircle(Circle l, Circle r);
   Vec2 getDisplacementRectFromCircle(Rect rect, Circle circle);
   Vec2 getDisplacementRectFromRect(Rect l, Rect r);
}
