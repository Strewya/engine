#pragma once
/********************************************
*	class:	Circle
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
	class Circle
	{
	public:
		Vec2 center;
		float radius;

		Circle();
		Circle(Vec2 center, float radius);

		void set(float cx, float cy, float r);
	};
}
