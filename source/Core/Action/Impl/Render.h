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

	SYSTEM(Render)
	{
	public:
		SYSTEM_CTOR(Render);
		
	protected:
		void frameUpdate(float dt);
		bool validateEntity(Entity& id) const;

	private:
		Graphics::RenderingQueue m_queue;
	};
}