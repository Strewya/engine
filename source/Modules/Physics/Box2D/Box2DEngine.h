#pragma once
/********************************************
*	class:	Box2DEngine
*	usage:
********************************************/
/******* common header *******/
#include <Engine/Defines.h>
#include <Engine/FwdDecl.h>
/******* extra headers if needed *******/
#include <Box2D/Box2D.h>
/******* end header inclusion *******/

namespace Physics
{
	class Box2DEngine
	{
	public:
		Box2DEngine() : m_world(b2Vec2(0,-9)) {}

		b2World m_world;

	};
}
