#pragma once
#define BOX2D_PHYSICS
/********************************************
	class:	PhysicsWrapper
	usage:
********************************************/
	/*** common header ***/
#include <Engine/Defines.h>
#include <Engine/FwdDecl.h>
	/*** extra headers if needed ***/
#ifdef BOX2D_PHYSICS
#include <Box2D/Box2D.h>
#endif
	/*** end header inclusion ***/


namespace Core
{
	class Box2DPhysicsSystem
	{
	public:
		b2World m_world;

	private:

	};



#ifdef BOX2D_PHYSICS
	typedef Box2DPhysicsSystem PhysicsSystem;
#endif
}
