#pragma once
/********************************************
*	class:	Collision check functions
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class Circle;
   class Rect;
   template<typename T>
   class Vec2;
   typedef Vec2<float> Vec2f;

   bool isPointInsideRect(Vec2f point, Rect rect);
   bool isPointInsideCircle(Vec2f point, Circle circle);
   bool isRectInsideRect(Rect inner, Rect outer);
   bool isCircleInsideCircle(Circle inner, Circle outer);
   bool isRectInsideCircle(Rect rect, Circle circle);
   bool isCircleInsideRect(Circle circle, Rect rect);

   bool isRectTouchingCircle(Rect rect, Circle circle);
   bool isCircleTouchingRect(Circle circle, Rect rect);
   bool isRectTouchingRect(Rect l, Rect r);
   bool isCircleTouchingCircle(Circle l, Circle r);
   bool isLineTouchingCircle(Vec2f A, Vec2f B, Circle circle);
}
