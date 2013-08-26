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
		SYSTEM_CTOR(ARender);
		
	protected:
		void onUpdate(float dt);

	private:
		Graphics::RenderingQueue m_queue;
	};
}