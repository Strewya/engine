#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Box2D/Box2D.h"
#include "Core/Action/Action.h"
	/*** end header inclusion ***/
	
namespace Core
{
	SYSTEM(Physics)
	{
	public:
		SYSTEM_CTOR(Physics);
		bool validateEntity(InstanceID id);

	protected:
		void onUpdate(float dt);
		void init();



	};
}