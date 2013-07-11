#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <list>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Subsystems/Graphics/RenderingQueue.h"
	/*** end header inclusion ***/

namespace Core
{
	class Entity;
	class GameContext;
	

	class ARender
	{
	public:
		bool Update(float dt, GameContext& context);

		bool AddEntity(Entity& e);
		bool AddEntity(InstanceID id);

		bool RemoveEntity(Entity& e);
		bool RemoveEntity(InstanceID id);

	private:
		Graphics::RenderingQueue _queue;
		std::list<InstanceID> _entities;
	};
}