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
#include <Util/Vec3.h>
/******* end headers *******/

namespace Core
{
	bool isPointInsideRect(const Vec2& point, const Rect& rect)
	{
		if(point.x < rect.center.x - rect.halfWidth) return false;
		if(point.x > rect.center.x + rect.halfWidth) return false;
		if(point.y < rect.center.y - rect.halfHeight) return false;
		if(point.y > rect.center.y + rect.halfHeight) return false;
		return true;
	}

	bool isPointInsideCircle(const Vec2& point, const Circle& circle)
	{
		return (Vec2::length(point - circle.center) <= circle.radius);
	}

	bool isRectInsideRect(const Rect& inner, const Rect& outer)
	{
		if(!isPointInsideRect({inner.left(), inner.top()}, outer)) return false;
		if(!isPointInsideRect({inner.left(), inner.bottom()}, outer)) return false;
		if(!isPointInsideRect({inner.right(), inner.top()}, outer)) return false;
		if(!isPointInsideRect({inner.right(), inner.bottom()}, outer)) return false;
		return true;
	}

	bool isCircleInsideCircle(const Circle& inner, const Circle& outer)
	{
		return (Vec2::length(inner.center - outer.center) <= outer.radius - inner.radius);
	}

	bool isRectInsideCircle(const Rect& rect, const Circle& circle)
	{
		if(!isPointInsideCircle({rect.left(), rect.top()}, circle)) return false;
		if(!isPointInsideCircle({rect.left(), rect.bottom()}, circle)) return false;
		if(!isPointInsideCircle({rect.right(), rect.top()}, circle)) return false;
		if(!isPointInsideCircle({rect.right(), rect.bottom()}, circle)) return false;
		return true;
	}

	bool isCircleInsideRect(const Circle& circle, const Rect& rect)
	{
		if(!isPointInsideRect(circle.center, rect)) return false;
		Vec2 pts[4]{circle.center, circle.center, circle.center, circle.center};
		pts[0].x -= circle.radius;
		pts[1].x += circle.radius;
		pts[2].y -= circle.radius;
		pts[3].y += circle.radius;
		for(auto i = 0; i < 4; ++i)
		{
			if(!isPointInsideRect(pts[i], rect))
			{
				return false;
			}
		}
		return true;
	}

	bool isRectTouchingCircle(const Rect& rect, const Circle& circle)
	{
		return isCircleTouchingRect(circle, rect);
	}

	bool isCircleTouchingRect(const Circle& circle, const Rect& rect)
	{
		if(circle.center.x + circle.radius < rect.left()) return false;
		if(circle.center.x - circle.radius > rect.right()) return false;
		if(circle.center.y + circle.radius < rect.bottom()) return false;
		if(circle.center.y - circle.radius > rect.top()) return false;
		return true;
	}

	bool isRectTouchingRect(const Rect& l, const Rect& r)
	{
		if(l.left() > r.right()) return false;
		if(l.right() < r.left()) return false;
		if(l.top() < r.bottom()) return false;
		if(l.bottom() > r.top()) return false;
		return true;
	}

	bool isCircleTouchingCircle(const Circle& l, const Circle& r)
	{
		return (Vec2::length(l.center - r.center) <= l.radius + r.radius);
	}
}
