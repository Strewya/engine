#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <list>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/Action/Action.h"
#include "Services/Graphics/RenderingQueue.h"
	/*** end header inclusion ***/

namespace Core
{
	class GameContext;

	SYSTEM(ARender)
	{
	public:
		bool onUpdate(float dt, GameContext& context);

	private:
		Graphics::RenderingQueue _queue;
	};
}