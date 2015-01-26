//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Util/Circle.h>
/******* C++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
	Circle::Circle()
		: Circle({0, 0}, 0)
	{}

	Circle::Circle(Vec2f c, float r)
		: center(c), radius(r)
	{}

	void Circle::set(float cx, float cy, float r)
	{
		center.set(cx, cy);
		radius = r;
	}
}
