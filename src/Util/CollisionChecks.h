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

	bool isPointInsideRect(const Vec2& point, const Rect& rect);
	bool isPointInsideCircle(const Vec2& point, const Circle& circle);
	bool isRectInsideRect(const Rect& inner, const Rect& outer);
	bool isCircleInsideCircle(const Circle& inner, const Circle& outer);
	bool isRectInsideCircle(const Rect& rect, const Circle& circle);
	bool isCircleInsideRect(const Circle& circle, const Rect& rect);

	bool isRectTouchingCircle(const Rect& rect, const Circle& circle);
	bool isCircleTouchingRect(const Circle& circle, const Rect& rect);
	bool isRectTouchingRect(const Rect& l, const Rect& r);
	bool isCircleTouchingCircle(const Circle& l, const Circle& r);
	bool isLineTouchingCircle(Vec2 A, Vec2 B, Circle circle);
}
