#pragma once
/********************************************
*	class:	
*	usage:
********************************************/
/******* common header *******/
#include <Engine/Defines.h>
#include <Engine/FwdDecl.h>
/******* extra headers if needed *******/
/******* end header inclusion *******/

#define BOX2D_ENGINE 0
#define BULLET_ENGINE 1

#define PHYSICS_ENGINE_TO_USE	BOX2D_ENGINE

#if PHYSICS_ENGINE_TO_USE == BOX2D_ENGINE

#include <Modules/Physics/Box2D/Box2DEngine.h>
#include <Modules/Physics/Box2D/Box2DAction.h>
#include <Modules/Physics/Box2D/Box2DState.h>

namespace Physics
{
	typedef Box2DEngine Engine;
}

#elif PHYSICS_ENGINE_TO_USE == BULLET_ENGINE

#include <Modules/Physics/Bullet/BulletEngine.h>

namespace Physics
{
	typedef BulletEngine Engine;
}

#endif



