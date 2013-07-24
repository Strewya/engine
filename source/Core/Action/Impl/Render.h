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
#include "Subsystems/Graphics/RenderingQueue.h"
	/*** end header inclusion ***/

namespace Core
{
	class Entity;
	class GameContext;
	

	class ARender : public Action
	{
	public:
		ARender();
		bool update(float dt, GameContext& context);

	private:
		Graphics::RenderingQueue _queue;
	};
}