//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Util/Transform.h>
/******* C++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
	Transform::Transform()
		: position(0,0), scale(1, 1), rotation(0)
	{}

	Transform::Transform(Vec2 p, Vec2 s, float r)
		: position(p), scale(s), rotation(r)
	{}
}