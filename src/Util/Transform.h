#pragma once
/********************************************
*	class:	Transform
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
	class Transform
	{
	public:
		Transform();
		Transform(Vec2 p, Vec2 s, float r);

		Vec2 position;
		Vec2 scale;
		float rotation;

	};
}