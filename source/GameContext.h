#ifndef CORE_GAME_CONTEXT_H_
#define CORE_GAME_CONTEXT_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <memory>
#include <set>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "ResourceLocator.h"
#include "ServiceLocator.h"
#include "Core/Action/ActionUpdater.h"
#include "Core/Entity/Entity.h"
#include "Util/Clock.h"
	/*** end header inclusion ***/

namespace Core
{
	class Engine;

	class GameContext
	{
	private:
		ServiceLocator _services;
		ResourceLocator _resources;
		ActionUpdater _actionMaster;
		Util::Timer _timer;
		Engine& _engine;
		std::set<std::unique_ptr<Entity>> _entities;
	
	public:
		GameContext(Engine& engine, const ServiceLocator& services, const ResourceLocator& resources);
		void Activate();
		void Deactivate();
		bool Update();
	};
}

#endif //CORE_GAME_CONTEXT_H_