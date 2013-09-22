#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <functional>
#include <list>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/Action/Action.h"
	/*** end header inclusion ***/

namespace Input { class Event; }

namespace Pong
{
	enum class Intent;

	SYSTEM(InputHandler)
	{
	public:
		SYSTEM_CTOR(InputHandler);
		bool validateEntity(Core::Entity& id);

	protected:
		void onUpdate(float dt);
		void init();

	private:
	};
}