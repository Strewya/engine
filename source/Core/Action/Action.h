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
		Action();

		virtual bool update(float dt, GameContext& context) = 0;

		bool registerEntity(InstanceID id);
		bool unregisterEntity(InstanceID id);

	protected:
		typedef std::unordered_set<InstanceID> EntityStorage_t;
		EntityStorage_t _entities;
	};
}