#pragma once
/********************************************
	class:	Action
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <unordered_set>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Core
{
	class GameContext;

	class Action
	{
	public:
		Action(uint32_t priority);

		virtual bool update(float dt, GameContext& context) = 0;

		bool registerEntity(InstanceID id);
		bool unregisterEntity(InstanceID id);
		uint32_t getPriority() const;

	protected:
		typedef std::unordered_set<InstanceID> EntityStorage_t;
		EntityStorage_t _entities;
		uint32_t _priority;
	};
}