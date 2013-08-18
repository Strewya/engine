#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <memory>
#include <set>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Box2D/Box2D.h"
#include "Engine/GameContextEvent.h"
#include "Engine/ResourceLocator.h"
#include "Engine/ServiceLocator.h"
#include "Core/Action/ActionIndex.h"
#include "Core/Action/ActionUpdater.h"
#include "Core/Entity/EntityFactory.h"
#include "Core/Entity/EntityPool.h"
#include "Core/Space/SpacePool.h"
#include "Services/Graphics/SpritesheetCache.h"
#include "Services/Graphics/ITextureCache.h"
#include "Util/CompositeTimer.h"
	/*** end header inclusion ***/

namespace Core
{
	enum ContextType
	{
		MENU,
		GAMEPLAY,
	};

	class GameContext
	{
	public:
		const ContextType Type;
		Util::CompositeTimer timer;
		ActionIndex actionIndex;
		ActionUpdater actionQueue;
		EntityPool entityPool;
		EntityFactory entityFactory;
		SpacePool spacePool;
		ServiceLocator& services;
		ResourceLocator& resources;
		b2World physicsWorld;
		Space activeEntities;

		GameContext(ContextType type, ServiceLocator& services, ResourceLocator& resources);
		virtual ~GameContext();

		bool update();

		virtual void destroy();
		virtual void onUpdate(float dt);
		/**
		 * This function is called every time the context goes from being inactive to being the active context.
		 * It should create all of the entities that it needs to work properly.
		 */
		virtual void activate() = 0;
		/**
		 * This function is called every time the context goes from being the active one to being inactive.
		 * It should destroy all objects that are created in the ACTIVATE function.
		 * NOTE: Think of a way to keep entities beyond the inactivity boundary.
		 * The onActivate function should take this into consideration when creating that particular entity to check whether it already exists.
		 */
		virtual void deactivate() = 0;
		/**
		 *	This function should be called only once to setup the actions this context is going to use.
		 */
		virtual void registerActions() = 0;
		/**
		 * This function should be called only once to setup the order in which the actions will be updated.
		 */
		virtual void setupActionQueue() = 0;
	};
}
