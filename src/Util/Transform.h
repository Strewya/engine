#pragma once
/********************************************
*	class:	Transform
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Util/Dimensional.h>
/******* end header inclusion *******/

namespace Core
{
	class Transform
	{
	public:
		Transform();

		Vec2 position;
		Vec2 scale;
		float rotation;

	};
}