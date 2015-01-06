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
	class Vec2;
	class Vec3;

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
}
